#include <string>
#include <vector>
#include <fstream>
#include <iostream>

int forward_euler(float (*function)(float, float), float y0, float step, float t_final, std::string filename) {
    std::vector<float> times = {0};
    std::vector<float> y_values = {y0};

    float t = 0;
    float y = y0;

    while (t < t_final) {
        float dy = function(t, y);
        t += step;
        y += dy * step;

        times.push_back(t);
        y_values.push_back(y);
    }

    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        outFile << "t,y\n";

        for (int i = 0; i < times.size(); i++){
            outFile << times[i] << "," << y_values[i] << "\n";
        }

        outFile.close();
    }
    
    return 0;
}