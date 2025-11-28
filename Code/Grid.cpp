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
    if (x >= 0 && x < rows && y >= 0 && y < cols) {
        temperature[x][y] += value;
    }
}

void Grid::setTemp(int x, int y, double value) {
    if (x >= 0 && x < rows && y >= 0 && y < cols) {
        temperature[x][y] = value;
    }
}

double Grid::getTemp(int x, int y) const {
    if (x >= 0 && x < rows && y >= 0 && y < cols) {
        return temperature[x][y];
    }
    return 0.0;
}

void Grid::update(DiffusionMethod method, LinkedList& sources) {
    // First, apply the heat from all sources to the grid
    Node* current = sources.getHead();
    while (current != nullptr) {
        applyHeat(current->data.x, current->data.y, current->data.intensity);
        current = current->next;
    }

    // Now, run the chosen diffusion algorithm from each source
    current = sources.getHead();
    while (current != nullptr) {
        switch (method) {
            case BFS:
                GraphTraversal::bfs(*this, current->data.x, current->data.y);
                break;
            case DFS:
                GraphTraversal::dfs(*this, current->data.x, current->data.y);
                break;
            case RECURSIVE:
                GraphTraversal::recursivePropagation(*this, current->data.x, current->data.y);
                break;
        }
        current = current->next;
    }
}

void Grid::display() const {
    std::cout << "Heatmap Display:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double temp = temperature[i][j];
            char c = ' ';
            if (temp > 90.0) c = '@';
            else if (temp > 75.0) c = '#';
            else if (temp > 60.0) c = 'O';
            else if (temp > 45.0) c = 'o';
            else if (temp > 30.0) c = '.';
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "Legend: . (30-45) o (45-60) O (60-75) # (75-90) @ (>90)\n\n";
}

int Grid::getRows() const {
    return rows;
}

int Grid::getCols() const {
    return cols;
}

double Grid::getHottestTemp() const {
    double hottest = temperature[0][0];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (temperature[i][j] > hottest) {
                hottest = temperature[i][j];
            }
        }
    }
    return hottest;
}

double Grid::getColdestTemp() const {
    double coldest = temperature[0][0];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (temperature[i][j] < coldest) {
                coldest = temperature[i][j];
            }
        }
    }
    return coldest;
}