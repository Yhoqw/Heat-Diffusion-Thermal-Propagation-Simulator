#include "GraphTraversal.h"
#include <queue>
#include <stack>
#include <iostream>

using namespace std;

// ------------------------------------------------------------
// BFS Traversal (Spreads outward level by level)
// ------------------------------------------------------------
void GraphTraversal::bfs(Grid& grid, int sx, int sy) {
    if (!grid.isValid(sx, sy)) {
        cout << "[BFS] Invalid starting point.\n";
        return;
    }

    int width = grid.getWidth();
    int height = grid.getHeight();

    vector<vector<bool>> visited(height, vector<bool>(width, false));

    queue<pair<int, int>> q;
    q.push({sx, sy});
    visited[sy][sx] = true;

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    cout << "[BFS] Traversal Order: ";

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        cout << "(" << x << "," << y << ") ";

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (grid.isValid(nx, ny) && !visited[ny][nx]) {
                visited[ny][nx] = true;
                q.push({nx, ny});
            }
        }
    }

    cout << "\n";
}


// ------------------------------------------------------------
// DFS Traversal (Deep-first exploration)
// ------------------------------------------------------------
void GraphTraversal::dfs(Grid& grid, int sx, int sy) {
    if (!grid.isValid(sx, sy)) {
        cout << "[DFS] Invalid starting point.\n";
        return;
    }

    int width = grid.getWidth();
    int height = grid.getHeight();

    vector<vector<bool>> visited(height, vector<bool>(width, false));

    stack<pair<int, int>> st;
    st.push({sx, sy});
    visited[sy][sx] = true;

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    cout << "[DFS] Traversal Order: ";

    while (!st.empty()) {
        auto [x, y] = st.top();
        st.pop();

        cout << "(" << x << "," << y << ") ";

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (grid.isValid(nx, ny) && !visited[ny][nx]) {
                visited[ny][nx] = true;
                st.push({nx, ny});
            }
        }
    }

    cout << "\n";
}


// ------------------------------------------------------------
// Recursive Heat Propagation (Simple DFS-style)
// ------------------------------------------------------------
// Applies a small diffusion effect recursively
void GraphTraversal::recursivePropagation(Grid& grid, int x, int y) {
    if (!grid.isValid(x, y))
        return;

    // Decay factor: each recursive step weakens the influence
    double currentTemp = grid.get(x, y);
    double newTemp = currentTemp * 0.9;   // 10% decay outward

    grid.set(x, y, newTemp);

    // Stop recursion when values become very small
    if (newTemp < 0.05)
        return;

    // Explore all 4 directions
    recursivePropagation(grid, x + 1, y);
    recursivePropagation(grid, x - 1, y);
    recursivePropagation(grid, x, y + 1);
    recursivePropagation(grid, x, y - 1);
}
