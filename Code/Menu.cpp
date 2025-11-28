#include "Menu.h"
#include "Grid.h"
#include "LinkedList.h"
#include "HeatSource.h"
#include "FileManager.h"

#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

// ------------------------------------------------------------
//  WRAPPER METHODS (parameterless — original skeleton support)
// ------------------------------------------------------------

void Menu::mainMenu() {
    Grid grid(20, 20);        // default grid
    LinkedList sources;       // empty heat source list
    mainMenu(grid, sources);
}

void Menu::adminMenu() {
    Grid grid(20, 20);
    LinkedList sources;
    adminMenu(grid, sources);
}

void Menu::viewerMenu() {
    Grid grid(20, 20);
    viewerMenu(grid);
}

// ------------------------------------------------------------
//  MAIN MENU WITH GRID + SOURCES
// ------------------------------------------------------------
void Menu::mainMenu(Grid& grid, LinkedList& sources) {
    while (true) {
        clearConsole();
        printHeader("HEAT DIFFUSION SIMULATOR");
        printMainOptions();

        int choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1:
            adminMenu(grid, sources);
            break;
        case 2:
            viewerMenu(grid);
            break;
        case 0:
            cout << "Exiting program...\n";
            return;
        default:
            cout << "Invalid option.\n";
            pressAnyKeyToContinue();
        }
    }
}

// ------------------------------------------------------------
//  ADMIN MENU
// ------------------------------------------------------------
void Menu::adminMenu(Grid& grid, LinkedList& sources) {
    while (true) {
        clearConsole();
        printHeader("ADMIN PANEL");
        printAdminOptions();

        int choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1:
            handleAddHeatSource(grid, sources);
            break;

        case 2:
            handleRemoveHeatSource(sources);
            break;

        case 3:
            handleRunSimulationStep(grid, sources);
            break;

        case 4:
            handleSaveState(grid, sources);
            break;

        case 5:
            handleLoadState(grid, sources);
            break;

        case 0:
            return;

        default:
            cout << "Invalid option.\n";
            pressAnyKeyToContinue();
        }
    }
}

// ------------------------------------------------------------
//  VIEWER MENU
// ------------------------------------------------------------
void Menu::viewerMenu(Grid& grid) {
    while (true) {
        clearConsole();
        printHeader("VIEWER MODE");
        printViewerOptions();

        int choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1:
            handleShowHeatmap(grid);
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice.\n";
            pressAnyKeyToContinue();
        }
    }
}

// ------------------------------------------------------------
//  ACTION HANDLERS
// ------------------------------------------------------------
void Menu::handleAddHeatSource(Grid& grid, LinkedList& sources) {
    cout << "--- Add New Heat Source ---\n";
    int x = readInt("Enter X coordinate: ");
    int y = readInt("Enter Y coordinate: ");
    double intensity = readDouble("Enter heat intensity: ");

    if (!grid.isValid(x, y)) {
        cout << "Invalid coordinates.\n";
        pressAnyKeyToContinue();
        return;
    }

    HeatSource hs(x, y, intensity);
    sources.addSource(hs);
    cout << "Heat source added.\n";

    pressAnyKeyToContinue();
}

void Menu::handleRemoveHeatSource(LinkedList& sources) {
    cout << "--- Remove Heat Source ---\n";
    int x = readInt("Enter X coordinate of source to remove: ");
    int y = readInt("Enter Y coordinate of source to remove: ");

    if (sources.removeSource(x, y)) {
        cout << "Heat source removed.\n";
    } else {
        cout << "No heat source found at that location.\n";
    }

    pressAnyKeyToContinue();
}

void Menu::handleRunSimulationStep(Grid& grid, LinkedList& sources) {
    cout << "--- Running Simulation Step ---\n";
    grid.applyHeatSources(sources);
    grid.update();
    cout << "Simulation updated.\n";
    pressAnyKeyToContinue();
}

void Menu::handleShowHeatmap(Grid& grid) {
    cout << "--- Current Heatmap ---\n";
    grid.print();
    pressAnyKeyToContinue();
}

void Menu::handleSaveState(Grid& grid, LinkedList& sources) {
    string filename = readString("Enter filename to save: ");
    FileManager::saveState(filename, grid, sources);
    cout << "State saved successfully.\n";
    pressAnyKeyToContinue();
}

void Menu::handleLoadState(Grid& grid, LinkedList& sources) {
    string filename = readString("Enter filename to load: ");
    if (FileManager::loadState(filename, grid, sources)) {
        cout << "State loaded successfully.\n";
    } else {
        cout << "Failed to load file.\n";
    }
    pressAnyKeyToContinue();
}

// ------------------------------------------------------------
//  INPUT UTILITIES
// ------------------------------------------------------------
int Menu::readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again.\n";
    }
}

double Menu::readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again.\n";
    }
}

string Menu::readString(const string& prompt) {
    cout << prompt;
    string s;
    cin >> s;
    return s;
}

void Menu::pressAnyKeyToContinue() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ------------------------------------------------------------
//  SCREEN HELPERS
// ------------------------------------------------------------

void Menu::clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Menu::printHeader(const string& title) {
    cout << "============================================\n";
    cout << " " << title << "\n";
    cout << "============================================\n\n";
}

void Menu::printMainOptions() {
    cout << "1. Admin Login\n";
    cout << "2. Viewer Login\n";
    cout << "0. Exit\n";
}

void Menu::printAdminOptions() {
    cout << "1. Add Heat Source\n";
    cout << "2. Remove Heat Source\n";
    cout << "3. Run Simulation Step\n";
    cout << "4. Save State\n";
    cout << "5. Load State\n";
    cout << "0. Back\n";
}

void Menu::printViewerOptions() {
    cout << "1. View Heatmap\n";
    cout << "0. Back\n";
}
