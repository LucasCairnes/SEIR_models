CC = c++

SHARED_FLAGS = -std=c++11 -O3 -Wall -undefined dynamic_lookup $(shell python3.13 -m pybind11 --includes) -shared -fPIC

EULER_SRC = forward_euler/seir_forward_euler.cpp
EULER_OUT = forward_euler/seir_forward_euler$(shell python3.13-config --extension-suffix)

MONTE_SRC = monte_carlo/seir_monte_carlo.cpp
MONTE_OUT = monte_carlo/seir_monte_carlo$(shell python3.13-config --extension-suffix)

.PHONY: all clean euler monte

all: monte euler

euler: $(EULER_SRC)
	$(CC) $(SHARED_FLAGS) $(EULER_SRC) -o $(EULER_OUT)

monte: $(MONTE_SRC)
	$(CC) $(SHARED_FLAGS) $(MONTE_SRC) -o $(MONTE_OUT)

clean:
	rm -f $(EULER_OUT) $(MONTE_OUT)