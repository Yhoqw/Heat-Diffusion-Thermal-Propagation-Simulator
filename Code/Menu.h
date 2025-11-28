#pragma once

#include <string>

// forward declarations to avoid including heavy headers in this header file
class Grid;
class LinkedList;

class Menu {
public:
    // --- Primary entry points (skeleton-preserved) ---
    // Original parameterless skeleton methods (kept for compatibility).
    static void mainMenu();
    static void adminMenu();
    static void viewerMenu();

    // --- Recommended overloads that accept the program state ---
    // Prefer to call these when integrating with the rest of the program.
    static void mainMenu(Grid& grid, LinkedList& sources);
    static void adminMenu(Grid& grid, LinkedList& sources);
    static void viewerMenu(Grid& grid);

private:
    // --- Small helper utilities used by the menu implementation ---
    // (Implemented in Menu.cpp)
    static int readInt(const std::string& prompt);
    static double readDouble(const std::string& prompt);
    static std::string readString(const std::string& prompt);
    static void pressAnyKeyToContinue();

    // Screen helpers
    static void clearConsole();
    static void printHeader(const std::string& title);
    static void printMainOptions();
    static void printAdminOptions();
    static void printViewerOptions();

    // Action helpers (high-level actions performed by menu items)
    static void handleAddHeatSource(Grid& grid, LinkedList& sources);
    static void handleRemoveHeatSource(LinkedList& sources);
    static void handleRunSimulationStep(Grid& grid, LinkedList& sources);
    static void handleShowHeatmap(Grid& grid);
    static void handleSaveState(Grid& grid, LinkedList& sources);
    static void handleLoadState(Grid& grid, LinkedList& sources);
};
