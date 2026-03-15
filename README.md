Compilation code for C++:

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) seir_forward_euler.cpp -o seir_forward_euler$(python3-config --extension-suffix) -undefined dynamic_lookup -fvisibility=hidden