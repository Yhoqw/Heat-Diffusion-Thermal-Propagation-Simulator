#include "Menu.h"
#include "Admin.h"
#include "Grid.h"
#include "LinkedList.h"
#include <iostream>

void Menu::mainMenu() {
    Grid grid(20, 20);
    LinkedList heatSources;

    std::cout << "\n=== HEAT DIFFUSION THERMAL PROPAGATION SIMULATOR ===" << std::endl;
    std::cout << "Welcome to the Heat Diffusion System" << std::endl;

    while (true) {
        std::cout << "\n=== MAIN MENU ===" << std::endl;
        std::cout << "1. Admin Access" << std::endl;
        std::cout << "2. Viewer Access" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter choice (1-3): ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                adminMenu(grid, heatSources);
                break;
            case 2:
                viewerMenu(grid, heatSources);
                break;
            case 3:
                std::cout << "Thank you for using the Heat Diffusion Simulator. Goodbye!" << std::endl;
                return;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

void Menu::adminMenu(Grid& grid, LinkedList& sources) {
    std::string password;
    std::cout << "\n=== ADMIN LOGIN ===" << std::endl;
    std::cout << "Enter admin password: ";
    std::cin >> password;

    Admin admin(grid, sources, "admin", "admin123");
    if (!admin.verifyPassword(password)) {
        std::cout << "Incorrect password! Access denied." << std::endl;
        return;
    }

    admin.displayMenu();
}

void Menu::viewerMenu(Grid& grid, LinkedList& sources) {
    std::cout << "\n=== VIEWER MODE ===" << std::endl;
    std::cout << "1. View Heatmap" << std::endl;
    std::cout << "2. View Heat Sources" << std::endl;
    std::cout << "3. Back to Main Menu" << std::endl;
    std::cout << "Enter choice (1-3): ";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            grid.display();
            break;
        case 2:
            sources.display();
            break;
        case 3:
            return;
        default:
            std::cout << "Invalid choice!" << std::endl;
    }
}

void Menu::adminMenu() {
    // Legacy function (not used)
}

void Menu::viewerMenu() {
    // Legacy function (not used)
}
