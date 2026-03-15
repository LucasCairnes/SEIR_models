#include <string>
#include <fstream>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;

float ds_dt(float i, float s, float beta) {
    return -beta * i * s;
}
float de_dt(float i, float s, float beta, float sigma, float e) {
    return beta * i * s - sigma * e;
}
float di_dt(float i, float sigma, float e, float gamma) {
    return sigma * e - gamma * i;
}
float dr_dt(float i, float gamma) {
    return gamma * i;
}

int forward_euler(float beta, float sigma, float gamma, float s_0, float e_0, float i_0, float r_0, float step, float t_final, const std::string& filename) {
    float t = 0.0, s = s_0, e = e_0, i = i_0, r = r_0;

    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << "time,susceptible,exposed,infected,recovered\n";

        while (t < t_final) {
            float ds = ds_dt(i, s, beta);
            s += ds * step;
            float de = de_dt(i, s, beta, sigma, e);
            e += de * step;
            float di = di_dt(i, sigma, e, gamma);
            i += di * step;
            float dr = dr_dt(i, gamma);
            r += dr * step;
        
            outFile << t << "," << s << "," << e << "," << i << "," << r <<"\n";
            t += step;
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