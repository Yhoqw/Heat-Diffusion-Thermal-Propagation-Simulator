#pragma once
#include "Grid.h"

class GraphTraversal {
public:
    static void bfs(Grid& grid, int sx, int sy);
    static void dfs(Grid& grid, int sx, int sy);
    static void recursivePropagation(Grid& grid, int x, int y);
};