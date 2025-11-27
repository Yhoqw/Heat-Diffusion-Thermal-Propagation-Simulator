#include "Grid.h"
#include <iostream>

Grid::Grid(int r, int c) : rows(r), cols(c) {
    temperature.resize(rows, std::vector<double>(cols, 25.0));
}

void Grid::initialize(double defaultTemp) {
    for (auto &row : temperature)
        std::fill(row.begin(), row.end(), defaultTemp);
}

void Grid::applyHeat(int x, int y, double value) {
    // Validation will be added
}

void Grid::update() {
    // To be implemented later
}   

double Grid::getTemp(int x, int y) const {
    return temperature[x][y];
}

void Grid::display() const {
    // ASCII heatmap later
}