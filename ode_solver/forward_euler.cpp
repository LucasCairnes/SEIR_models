#include <string>
#include <fstream>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;

int forward_euler(std::function<float(float, float)>& function, float y0, float step, float t_final, const std::string& filename) {
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
    m.def("forward_euler", &forward_euler, "A forward Euler ODE solver",
        py::arg("function"),
        py::arg("y0"),
        py::arg("step"),
        py::arg("t_final"),
        py::arg("filename"));
}