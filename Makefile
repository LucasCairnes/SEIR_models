CXX = GCC
CXXFLAGS = -O3 -Wall -shared -std=c++11 -shared -fPIC -fvisibility=hidden
INCLUDES = $(shell python3 -m pybind11 --includes)
LDFLAGS = -undefined dynamic_lookup
SRC = seir_forward_euler.cpp
TARGET = seir_forward_euler$(shell python3-config --extension-suffix)

all: 
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET) $(LDFLAGS)