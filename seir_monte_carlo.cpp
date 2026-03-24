#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>

struct Agent {
    int x_pos, y_pos, state;
    Agent(int x, int y, int s)
        : x_pos(x), y_pos(y), state(s) {}
}; // Using a struct since all variables are public

class System{
    private:
        int length;
        int susceptible, exposed, infected, recovered;
        float beta, sigma, gamma; // Storing variables which will be referenced by various functions

        std::vector<int> lattice; // 1D faster than 2D. Integers instead of pointers so the agents vector can be shuffled to eliminate biases
        std::vector<Agent> agents;

        std::mt19937 rng {42};
        std::uniform_int_distribution<int> creation_rng;
        std::uniform_int_distribution<int> movement_rng;
        std::uniform_real_distribution<float> prob_rng; // Initialising root engine and more specific rngs
        
        int get_index(int x_value, int y_value);
        int random_index();
        int random_movement();
        int random_state(float probability);
        int boundary_check(int coord);
        void populate_lattice(int agent_count, float s_0, float e_0, float i_0, float r_0);
        void move_agent(Agent &agent, int x_destination, int y_destination);
        void run_sim(int MCS, const std::string& filename);
        void update_state(Agent &agent); // Initialising all functions, ensuring to pass references to the agent objects

    public:
        System(int length, int agent_count, int MCS, float s_0, float e_0, float i_0, float r_0, float beta, float sigma, float gamma, const std::string& filename)
            : length(length), creation_rng(0, length - 1), movement_rng(-1, 1), prob_rng(0, 1), beta(beta), sigma(sigma), gamma(gamma) { // Outlining the required inputs and storing the frequently used variables
            lattice.resize(length*length, 0);
            populate_lattice(agent_count, s_0, e_0, i_0, r_0);
            run_sim(MCS, filename); // Calling the main functions to initialise and run the sim
        }
};

int System::random_index() {
    return creation_rng(rng);
}

int System::random_movement() {
    return movement_rng(rng);
}

int System::random_state(float probability) {
    if (prob_rng(rng) > probability) return 1;
    return 0; // Functionalising the random elements of the sim. Making the state function to work for all states
}

int System::get_index(int x_value, int y_value) {
    return (x_value * length) + y_value;
} // Function to convert 2D -> 1D

void System::populate_lattice(int agent_count, float s_0, float e_0, float i_0, float r_0) {
    int x_val, y_val, state;
    susceptible = static_cast<int>(std::round(agent_count * s_0));
    exposed = static_cast<int>(std::round(agent_count * e_0));
    infected = static_cast<int>(std::round(agent_count * i_0));
    recovered = agent_count - susceptible - exposed - infected; // Using left over agents to avoid rounding errors

    agents.reserve(agent_count); // Reserving the memory to prevent reallocation    

    for (int j = 0; j < agent_count; j++) {
        if (j < susceptible) state = 1;
        else if (j < susceptible + exposed) state = 2;
        else if (j < susceptible + exposed + infected) state = 3;
        else state = 4;
        do {
            x_val = random_index();
            y_val = random_index();
        } while (lattice[get_index(x_val, y_val)] != 0); // Checks until it finds an empty index

        agents.emplace_back(x_val, y_val, state);
        lattice[get_index(x_val, y_val)] = state; // Updating agents vector and lattice
    }
}

int System::boundary_check(int coord){
    if (coord < 0) return length - 1;
    if (coord >= length) return 0;
    return coord; // Loops index if exceeding boundary
}

void System::move_agent(Agent &agent, int x_destination, int y_destination) {
    x_destination = boundary_check(x_destination);
    y_destination = boundary_check(y_destination);

    if (lattice[get_index(x_destination, y_destination)] == 0) {
        lattice[get_index(x_destination, y_destination)] = lattice[get_index(agent.x_pos, agent.y_pos)];
        lattice[get_index(agent.x_pos, agent.y_pos)] = 0; // Updates position of the agent and then empties previous location
    
        agent.x_pos = x_destination;
        agent.y_pos = y_destination;
    }
}

void System::update_state(Agent &agent) {
    int current_state = agent.state;

    if (current_state == 4) return;
    
    int infected_neighbours = 0, state_change = 0;
    int agent_x = agent.x_pos, agent_y = agent.y_pos;

    if (current_state == 1) {
        if (lattice[get_index(boundary_check(agent_x + 1), agent_y)] == 3) infected_neighbours++;
        if (lattice[get_index(boundary_check(agent_x -1), agent_y)] == 3) infected_neighbours++;
        if (lattice[get_index(boundary_check(agent_x), agent_y + 1)] == 3) infected_neighbours++;
        if (lattice[get_index(boundary_check(agent_x), agent_y - 1)] == 3) infected_neighbours++; // Counts neighbours

        if (infected_neighbours == 0) return;

        int k = 0;
        do {
            state_change = random_state(beta);
        } while (state_change == 0 && k < infected_neighbours );
    }
    
    else {
        if (current_state == 2) state_change = random_state(sigma);
        else if (current_state == 3) state_change = random_state(gamma);
    }

    if (state_change == 1) { // Update state and counters if state updates
        agent.state = current_state + 1;
        lattice[get_index(agent_x, agent_y)] = current_state + 1;

        if (current_state == 1) {susceptible--; exposed++;}
        else if (current_state == 2) {exposed--; infected++;}
        else if (current_state == 3) {infected--; recovered++;}
    } 
}

void System::run_sim(int MCS, const std::string& filename) {
    int step = 0;
    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << "Monte Carlo step,susceptible,exposed,infected,recovered\n";
        outFile << step << "," << susceptible << "," << exposed << "," << infected << "," << recovered <<"\n";

        for (int k = 0; k < MCS; k++) {
            std::shuffle(agents.begin(), agents.end(), rng); // Shuffles to eliminate biases to the first agents in the vector
            for (Agent& curr_agent : agents) {
                move_agent(curr_agent, curr_agent.x_pos + random_movement(), curr_agent.y_pos + random_movement());
            } // Perform the random movements

            for (Agent& curr_agent : agents) {
                update_state(curr_agent);
            } // Update state due to neighbours

            step++;
            outFile << step << "," << susceptible << "," << exposed << "," << infected << "," << recovered <<"\n";
        }

        outFile.close();
    }
}