#pragma once
#include "Grid.h"
#include "LinkedList.h"

class FileManager {
public:
    static void saveState(Grid& grid, LinkedList& list);
    static void loadState(Grid& grid, LinkedList& list);
};
