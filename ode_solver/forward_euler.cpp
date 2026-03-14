#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <pybind11/pybind11.h>

int forward_euler(float (*function)(float, float), float y0, float step, float t_final, std::string filename) {
    std::vector<float> times = {0};
    std::vector<float> y_values = {y0};

    float t = 0;
    float y = y0;

    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << "t,y\n";

        while (t < t_final) {
            float dy = function(t, y);
            t += step;
            y += dy * step;

            outFile << t << "," << y << "\n";
        }

        outFile.close();
    }
    
    return 0;
}

PYBIND11_MODULE(ode_solver, m) {
    m.doc() = "A module for solving ODEs.";
    m.def("forward_euler", &forward_euler, "A forward Euler ODE solver");
}