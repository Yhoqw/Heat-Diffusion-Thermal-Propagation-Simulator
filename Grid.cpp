#include "Grid.h"
#include <algorithm>
#include <queue>
#include <stack>

Grid::Grid(int r, int c, float a, float cool) : rows(r), cols(c), alpha(a), cooling(cool) {
    grid.resize(rows, std::vector<float>(cols, 0.0f));
}

void Grid::add_heat_direct(int x, int y, float temperature) {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        grid[y][x] = temperature;
    }
}

void Grid::inject_sources() {
    sources.for_each([this](const HeatSource* s){
        if (s->x >= 0 && s->x < cols && s->y >= 0 && s->y < rows) {
            // We add the source temperature (max) — you can blend instead
            grid[s->y][s->x] = std::max(grid[s->y][s->x], s->temperature);
        }
    });         
}

void Grid::step() {
    if (mode == MODE_NORMAL) {
        inject_sources();

        std::vector<std::vector<float>> new_grid = grid;

        for (int i = 1; i < rows - 1; i++) {
            for (int j = 1; j < cols - 1; j++) {
                float avg_neighbor = (grid[i-1][j] + grid[i+1][j] +
                                    grid[i][j-1] + grid[i][j+1]) / 4.0f;

                new_grid[i][j] += alpha * (avg_neighbor - grid[i][j]);
            }
        }

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                new_grid[i][j] *= cooling;

        grid = new_grid;
    }
    else if (mode == MODE_BFS) {
        step_bfs();
    }
    else if (mode == MODE_DFS) {
        step_dfs();
    }    
}

float Grid::get_temperature(int x, int y) const {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        return grid[y][x];
    }
    return 0.0f;
}

std::vector<float> Grid::get_all_temperatures() const {
    std::vector<float> temps;
    temps.reserve(rows * cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            temps.push_back(grid[i][j]);
        }
    }
    return temps;
}

void Grid::step_bfs() {
    inject_sources();

    std::vector<std::vector<float>> new_grid = grid;
    std::queue<std::pair<int,int>> q;

    // enqueue all heat sources
    sources.for_each([&](const HeatSource* s){
        q.push({s->x, s->y});
    });

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        float current_temp = grid[y][x];
        float spread_temp = current_temp * alpha;

        if (spread_temp < 0.1f) continue; // threshold

        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                if (spread_temp > new_grid[ny][nx]) {
                    new_grid[ny][nx] = spread_temp;
                    q.push({nx, ny});
                }
            }
        }
    }

    grid = new_grid;    
}

void Grid::step_dfs() {
    inject_sources();

    std::vector<std::vector<float>> new_grid = grid;
    std::stack<std::pair<int,int>> st;

    // push all heat sources
    sources.for_each([&](const HeatSource* s){
        st.push({s->x, s->y});
    });

    while (!st.empty()) {
        auto [x, y] = st.top();
        st.pop();

        float current_temp = grid[y][x];
        float spread_temp = current_temp * alpha;

        if (spread_temp < 0.1f) continue;

        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                if (spread_temp > new_grid[ny][nx]) {
                    new_grid[ny][nx] = spread_temp;
                    st.push({nx, ny});
                }
            }
        }
    }

    grid = new_grid;  
}  

int Grid::get_rows() const {
    return rows;
}

int Grid::get_cols() const {
    return cols;
}

