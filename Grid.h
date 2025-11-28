#pragma once
#include <vector>

class Grid {
private:
    int rows, cols;
    std::vector<std::vector<double> > temperature;

public:
    Grid(int r = 20, int c = 20);

    void initialize(double defaultTemp = 25.0);
    void applyHeat(int x, int y, double value);
    void update();
    double getTemp(int x, int y) const;
    void display() const;
};

