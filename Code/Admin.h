#pragma once
#include "User.h"
#include "Grid.h"
#include "LinkedList.h"

class Admin : public User {
private:
    Grid& grid;
    LinkedList& sources;

public:
    Admin(Grid& g, LinkedList& s);
    void addHeatSource();
    void viewHeatmap() override;
};