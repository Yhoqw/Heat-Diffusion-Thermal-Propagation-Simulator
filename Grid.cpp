#include "Grid.h"
#include <iostream>
#include <iomanip>
#include <algorithm> // for std::fill

// Constructor
Grid::Grid(int r, int c) : rows(r), cols(c) {
    temperature.resize(rows, std::vector<double>(cols, 25.0)); // default 25°C
}

// Initialize the grid with a default temperature
void Grid::initialize(double defaultTemp) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            temperature[i][j] = defaultTemp;
        }
    }
}

// Apply heat at a specific cell
void Grid::applyHeat(int x, int y, double value) {
    if (x >= 0 && x < rows && y >= 0 && y < cols)
        temperature[x][y] += value;
}

// Simple diffusion update: average with neighbors
void Grid::update() {
    std::vector<std::vector<double> > newTemp = temperature;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double sum = temperature[i][j];
            int count = 1;

            if (i > 0) { sum += temperature[i - 1][j]; ++count; }
            if (i < rows - 1) { sum += temperature[i + 1][j]; ++count; }
            if (j > 0) { sum += temperature[i][j - 1]; ++count; }
            if (j < cols - 1) { sum += temperature[i][j + 1]; ++count; }

            newTemp[i][j] = sum / count;
        }
    }

    temperature = newTemp;
}

// Get temperature at a specific cell
double Grid::getTemp(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < cols)
        return temperature[x][y];
    return 0.0;
}

// Display the grid as a simple ASCII table
void Grid::display() const {
    std::cout << std::fixed << std::setprecision(1);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            std::cout << std::setw(6) << temperature[i][j] << " ";
        std::cout << std::endl;
    }
}

