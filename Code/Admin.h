#pragma once
#include "User.h"
#include "Grid.h"
#include "LinkedList.h"

class Admin : public User {
private:
    Grid& grid;
    LinkedList& sources;

public:
    Admin(Grid& g, LinkedList& s, const std::string& name = "admin", const std::string& pass = "admin123");
    void addHeatSource();
    void removeHeatSource();
    void viewHeatmap(const Grid& grid) override;
    void viewHeatSources() const;
    void initializeGrid(double tempValue);
    void simulateHeatDiffusion();
    void displayStats() const;
    void displayMenu() override;
};