#pragma once
#include <vector>
#include "LinkedList.h" // Required for heat sources
#include "GraphTraversal.h" // Required for diffusion algorithms
#include "Sorting.h"        // Required for hottest/coldest queries

enum DiffusionMethod {
    BFS,
    DFS,
    RECURSIVE
};

class Grid {
private:
    int rows, cols;
    std::vector<std::vector<double>> temperature;
    std::vector<double> getTemperaturesAsVector() const;

public:
    Grid(int r = 20, int c = 20);
    void initialize(double defaultTemp = 25.0);
    void applyHeat(int x, int y, double value);
    void setTemp(int x, int y, double value);
    void update(DiffusionMethod method, LinkedList& sources);
    double getTemp(int x, int y) const;
    void display() const;
    int getRows() const;
    int getCols() const;
    double getHottestTemp() const;
    double getColdestTemp() const;
};
