CC = c++
CFLAGS = -std=c++11 -O3 -Wall -undefined dynamic_lookup $(shell python3.13 -m pybind11 --includes) -shared -fPIC
SRC = seir_forward_euler.cpp
OUT = seir_forward_euler$(shell python3.13-config --extension-suffix)

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)