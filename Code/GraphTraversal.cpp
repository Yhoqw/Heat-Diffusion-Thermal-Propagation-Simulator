#include "GraphTraversal.h"
#include "Grid.h"
#include <vector>
#include <queue>
#include <stack>

// A simple decay factor for heat propagation
const double DECAY_FACTOR = 0.8;

void GraphTraversal::bfs(Grid& grid, int sx, int sy) {
    int rows = grid.getRows();
    int cols = grid.getCols();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;

    double startTemp = grid.getTemp(sx, sy);
    if (startTemp <= 0) startTemp = 100.0; // Default start temp if not already hot
    grid.setTemp(sx, sy, startTemp);

    q.push({sx, sy});
    visited[sx][sy] = true;

    int dr[] = {-1, 1, 0, 0}; // Directions for neighbors
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        std::pair<int, int> curr = q.front();
        q.pop();
        double currentTemp = grid.getTemp(curr.first, curr.second);

        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) {
                visited[nr][nc] = true;
                grid.setTemp(nr, nc, currentTemp * DECAY_FACTOR);
                q.push({nr, nc});
            }
        }
    }
}

void GraphTraversal::dfs(Grid& grid, int sx, int sy) {
    int rows = grid.getRows();
    int cols = grid.getCols();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::stack<std::pair<int, int>> s;

    double startTemp = grid.getTemp(sx, sy);
    if (startTemp <= 0) startTemp = 100.0;
    grid.setTemp(sx, sy, startTemp);

    s.push({sx, sy});
    visited[sx][sy] = true;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!s.empty()) {
        std::pair<int, int> curr = s.top();
        s.pop();
        double currentTemp = grid.getTemp(curr.first, curr.second);

        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) {
                visited[nr][nc] = true;
                grid.setTemp(nr, nc, currentTemp * DECAY_FACTOR);
                s.push({nr, nc});
            }
        }
    }
}

// Helper for the recursive propagation
void recursiveStep(Grid& grid, int x, int y, std::vector<std::vector<bool>>& visited, double currentTemp) {
    int rows = grid.getRows();
    int cols = grid.getCols();

    if (x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y] || currentTemp <= 1.0) {
        return;
    }

    visited[x][y] = true;
    grid.setTemp(x, y, currentTemp);

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        recursiveStep(grid, x + dr[i], y + dc[i], visited, currentTemp * DECAY_FACTOR);
    }
}

void GraphTraversal::recursivePropagation(Grid& grid, int x, int y) {
    int rows = grid.getRows();
    int cols = grid.getCols();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    
    double startTemp = grid.getTemp(x, y);
    if (startTemp <= 0) startTemp = 100.0;

    recursiveStep(grid, x, y, visited, startTemp);
}
