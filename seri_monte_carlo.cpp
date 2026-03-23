#include <vector>
#include <random>
#include <cmath>

struct Agent {
    int x_pos, y_pos, state;
    Agent(int x, int y, int s)
        : x_pos(x), y_pos(y), state(s) {}
};

class System{
    private:
        int side_length, agent_count, MCS;
        int susceptible, exposed, infected, recovered;
        std::vector<std::vector<Agent *>> lattice;
        std::mt19937 rng {42};
        std::uniform_int_distribution<int> unii;
        std::vector<Agent> agents;
        
        int get_random_index();
        void populate_lattice(int agent_count, float s, float e, float i);
        int boundary_check(int coord);
        void move_agent(Agent &agent, int x_destination, int y_destination);

    public:
        System(int side_length, int agent_count, int MCS, float s, float e, float i, float r)
            : side_length(side_length), agent_count(agent_count), MCS(MCS), unii(0, side_length - 1) {
            lattice.resize(side_length, std::vector<Agent*>(side_length, nullptr));
        }
};

int System::get_random_index(){
    return unii(rng);
}

void System::populate_lattice(int agent_count, float s, float e, float i) {
    int x_val, y_val, state;
    susceptible = static_cast<int>(std::round(agent_count * s));
    exposed = static_cast<int>(std::round(agent_count * e));
    infected = static_cast<int>(std::round(agent_count * i));
    recovered = agent_count - susceptible - exposed - infected;

    agents.reserve(agent_count);

    for (int j = 0; j < agent_count; j++) {
        if (j < susceptible) state = 1;
        else if (j < susceptible + exposed) state = 2;
        else if (j < susceptible + exposed + infected) state = 3;
        else state = 4;
        do {
            x_val = get_random_index();
            y_val = get_random_index();
        } while (lattice[x_val][y_val] != 0);

        agents.emplace_back(x_val, y_val, state);
        lattice[x_val][y_val] = &agents.back();
    }
}

int System::boundary_check(int coord){
    if (coord < 0) return side_length - 1;
    if (coord >= side_length) return 0;
    return coord;
}

void System::move_agent(Agent &agent, int x_destination, int y_destination) {
    x_destination = boundary_check(x_destination);
    y_destination = boundary_check(y_destination);

    if (lattice[x_destination][y_destination] == nullptr) {
        lattice[x_destination][y_destination] = lattice[agent.x_pos][agent.y_pos];
        lattice[agent.x_pos][agent.y_pos] = nullptr;
    
        agent.x_pos = x_destination;
        agent.y_pos = y_destination;
    }
}