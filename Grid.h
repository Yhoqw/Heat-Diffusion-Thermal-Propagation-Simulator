#pragma once
#include <vector>
#include "HeatSourceList.h"

enum SimulationMode {
    MODE_NORMAL,
    MODE_BFS,
    MODE_DFS
};

/* Grid Class
    Stores a 2D vector grid[y][x] representing temperatures.

Supports:
    Adding heat at any cell

    Simulating diffusion using the formula:
    T_new = T_old + α(N_avg - T_old)

    Cooling factor applied uniformly every step
*/
class Grid {
private:
    int rows;
    int cols;
    float alpha;
    float cooling;
    std::vector<std::vector<float>> grid;

public:
    SimulationMode mode = MODE_NORMAL;
    HeatSourceList sources;

    Grid(int r, int c, float a = 0.1f, float cool = 0.7f);

    void add_heat_direct(int x, int y, float temperature);
    void inject_sources();
    void step();

    //getters
    float get_temperature(int x, int y) const;
    std::vector<float> get_all_temperatures() const;

    int get_rows() const;
    int get_cols() const;

    // Traversal Algorithms
    void step_bfs();
    void step_dfs();
};