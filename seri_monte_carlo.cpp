#include <vector>
#include <random>
#include <cmath>
using namespace std;

class Agent {
    public:
        int x_pos, y_pos, state;
        Agent(int x, int y, int s)
            : x_pos(x), y_pos(y), state(s) {}
};

class System{
    private:
        int side_length;
        vector<vector<int>> lattice;
        mt19937 rng {42};
        uniform_int_distribution<int> unii;
        vector<Agent> agents;

    public:
        System(int length) : side_length(length), unii(0, length - 1) {
            lattice.resize(length, std::vector<int>(length, 0));
        }

        int get_random_index() {
            return unii(rng);
        };

        void populate_lattice(int agent_count, float susceptible);
        int boundary_check(int coord);
        void move_agent(Agent agent, int x_destination, int y_destination);
};

void System::populate_lattice(int agent_count, float susceptible) {
    int x_val, y_val, state;
    int susceptible_count = static_cast<int>(std::round(agent_count * susceptible));

    for (int i = 0; i < side_length; i++) {
        if (i < susceptible_count) {
            state = 1;
        } else {
            state = 2;
        };

        do {
            int x_val = get_random_index();
            int y_val = get_random_index();
        } while (lattice[x_val][y_val] != 0);

        lattice[x_val][y_val] = state;
        agents.push_back(Agent(x_val, y_val, state));
    }
};

int System::boundary_check(int coord){
    if (coord < 0) return side_length - 1;
    if (coord >= side_length) return 0;
    return coord;
}

void System::move_agent(Agent agent, int x_destination, int y_destination) {
    int x_destination = boundary_check(x_destination);
    int y_destination = boundary_check(y_destination);

    if (lattice[x_destination][y_destination] != 0) {
        lattice[agent.x_pos][agent.y_pos] = 0;
        lattice[x_destination][y_destination] = agent.state;
    
        agent.x_pos = x_destination;
        agent.y_pos = y_destination;
    }
};