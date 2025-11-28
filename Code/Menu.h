#pragma once

class Grid;
class LinkedList;

class Menu {
public:
    static void mainMenu();
    static void adminMenu(Grid& grid, LinkedList& sources);
    static void viewerMenu(Grid& grid, LinkedList& sources);

private:
    // Legacy method signatures for backward compatibility
    static void adminMenu();
    static void viewerMenu();
};