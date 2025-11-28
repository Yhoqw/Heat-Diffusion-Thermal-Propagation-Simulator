#include "Admin.h"
#include <iostream>
#include <iomanip>

Admin::Admin(Grid& g, LinkedList& s, const std::string& name, const std::string& pass)
    : User(name, pass), grid(g), sources(s) {}

void Admin::addHeatSource() {
    int x, y;
    double intensity;

    std::cout << "\n--- Add Heat Source ---" << std::endl;
    std::cout << "Enter X coordinate (0-" << grid.getRows() - 1 << "): ";
    std::cin >> x;
    std::cout << "Enter Y coordinate (0-" << grid.getCols() - 1 << "): ";
    std::cin >> y;
    std::cout << "Enter intensity value: ";
    std::cin >> intensity;

    // Validate coordinates
    if (x < 0 || x >= grid.getRows() || y < 0 || y >= grid.getCols()) {
        std::cout << "Error: Coordinates out of bounds!" << std::endl;
        return;
    }

    HeatSource newSource(x, y, intensity);
    sources.addSource(newSource);
    std::cout << "Heat source added at (" << x << ", " << y << ") with intensity " 
              << intensity << std::endl;
}

void Admin::removeHeatSource() {
    int x, y;

    std::cout << "\n--- Remove Heat Source ---" << std::endl;
    std::cout << "Enter X coordinate: ";
    std::cin >> x;
    std::cout << "Enter Y coordinate: ";
    std::cin >> y;

    sources.removeSource(x, y);
}

void Admin::viewHeatmap(const Grid& grid) {
    std::cout << "\n";
    grid.display();
}

void Admin::viewHeatSources() const {
    sources.display();
}

void Admin::initializeGrid(double tempValue) {
    std::cout << "\n--- Initialize Grid ---" << std::endl;
    grid.initialize(tempValue);
    std::cout << "Grid initialized with temperature: " << tempValue << " C" << std::endl;
}

void Admin::simulateHeatDiffusion() {
    std::cout << "\n--- Simulate Heat Diffusion ---" << std::endl;
    std::cout << "Select diffusion method:" << std::endl;
    std::cout << "1. BFS (Breadth-First Search)" << std::endl;
    std::cout << "2. DFS (Depth-First Search)" << std::endl;
    std::cout << "3. Recursive Propagation" << std::endl;
    std::cout << "Enter choice (1-3): ";

    int choice;
    std::cin >> choice;

    DiffusionMethod method;
    switch (choice) {
        case 1:
            method = BFS;
            std::cout << "Running BFS diffusion..." << std::endl;
            break;
        case 2:
            method = DFS;
            std::cout << "Running DFS diffusion..." << std::endl;
            break;
        case 3:
            method = RECURSIVE;
            std::cout << "Running Recursive diffusion..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
    }

    grid.update(method, sources);
    std::cout << "Heat diffusion simulation complete!" << std::endl;
}

void Admin::displayStats() const {
    std::cout << "\n=== System Statistics ===" << std::endl;
    std::cout << "Grid Size: " << grid.getRows() << " x " << grid.getCols() << std::endl;
    std::cout << "Total Heat Sources: " << sources.getCount() << std::endl;
    std::cout << "Hottest Temperature: " << grid.getHottestTemp() << " C" << std::endl;
    std::cout << "Coldest Temperature: " << grid.getColdestTemp() << " C" << std::endl;
    std::cout << std::string(30, '=') << std::endl << std::endl;
}

void Admin::displayMenu() {
    while (true) {
        std::cout << "\n=== ADMIN MENU (User: " << username << ") ===" << std::endl;
        std::cout << "1. Initialize Grid" << std::endl;
        std::cout << "2. Add Heat Source" << std::endl;
        std::cout << "3. Remove Heat Source" << std::endl;
        std::cout << "4. View Heat Sources" << std::endl;
        std::cout << "5. Simulate Heat Diffusion" << std::endl;
        std::cout << "6. View Heatmap" << std::endl;
        std::cout << "7. Display Statistics" << std::endl;
        std::cout << "8. Logout" << std::endl;
        std::cout << "Enter choice (1-8): ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                double temp;
                std::cout << "Enter default temperature for grid: ";
                std::cin >> temp;
                initializeGrid(temp);
                break;
            }
            case 2:
                addHeatSource();
                break;
            case 3:
                removeHeatSource();
                break;
            case 4:
                viewHeatSources();
                break;
            case 5:
                simulateHeatDiffusion();
                break;
            case 6:
                viewHeatmap(grid);
                break;
            case 7:
                displayStats();
                break;
            case 8:
                std::cout << "Logging out..." << std::endl;
                return;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}
