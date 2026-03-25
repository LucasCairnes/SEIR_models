#include <string>
#include <fstream>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11; // To condense the pybind args

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
} // The seir differential equations as easily callable functions

int seir_forward_euler(float beta, float sigma, float gamma, float s_0, float e_0, float i_0, float r_0, float step, float t_final, const std::string& filename) {
    float t = 0.0, s = s_0, e = e_0, i = i_0, r = r_0;

    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << "time,susceptible,exposed,infected,recovered\n";
        outFile << t << "," << s << "," << e << "," << i << "," << r <<"\n"; // Writing intial values

        while (t < t_final) {
            float ds = ds_dt(i, s, beta);
            float de = de_dt(i, s, beta, sigma, e);
            float di = di_dt(i, sigma, e, gamma);
            float dr = dr_dt(i, gamma); // Ensuring to pass the seir variables in before updating them

            s += ds * step;
            e += de * step;
            i += di * step;
            r += dr * step;
            
            t += step;
            outFile << t << "," << s << "," << e << "," << i << "," << r <<"\n";
        }

        outFile.close();
    }
    
    return 0;
}

PYBIND11_MODULE(seir_forward_euler, m) {
    m.doc() = "A module for solving SEIR ODEs.";
    m.def("forward_euler", &seir_forward_euler, "A function to solve the SEIR ODEs with the forward Euler method.", // Brief descriptions of the module and function
        py::arg("beta"),
        py::arg("sigma"),
        py::arg("gamma"),
        py::arg("s_0"),
        py::arg("e_0"),
        py::arg("i_0"),
        py::arg("r_0"),
        py::arg("step"),
        py::arg("t_final"),
        py::arg("filename") // Defining pybind args to allow args to be easily using in python, e.g. beta = 1.0
    );
}