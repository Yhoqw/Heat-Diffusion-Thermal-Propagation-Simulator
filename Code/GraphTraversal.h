#pragma once
#include "Grid.h"
#include <queue>
#include <stack>

class GraphTraversal {
public:

    // -------------------------------------------------------------------------
    // Breadth-First Search (BFS)
    // Used for layer-by-layer thermal propagation.
    // sx, sy = starting coordinates
    // -------------------------------------------------------------------------
    static void bfs(Grid& grid, int sx, int sy);

    // -------------------------------------------------------------------------
    // Depth-First Search (DFS)
    // Used for deep-path propagation (stack-based).
    // sx, sy = starting coordinates
    // -------------------------------------------------------------------------
    static void dfs(Grid& grid, int sx, int sy);

    // -------------------------------------------------------------------------
    // Recursive Flood-Fill / Propagation
    // Uses recursion instead of stack/queue.
    // x, y = current cell
    // -------------------------------------------------------------------------
    static void recursivePropagation(Grid& grid, int x, int y);

private:

    // Utility: Check if a cell is valid inside the grid.
    static bool isValid(const Grid& grid, int x, int y);

    // Utility: Apply heat diffusion rule when visiting a cell.
    // This updates the temperature of the (x, y) cell according to your model.
    static void applyHeat(Grid& grid, int x, int y);
};
