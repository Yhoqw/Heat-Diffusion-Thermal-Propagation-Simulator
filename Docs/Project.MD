# Project Specification for AI Agents

This document describes the architecture, goals, and components of the current project. It is intended to guide AI agents and automated assistants in understanding and extending the existing codebase.

---

## **1. Project Overview**

This project is a C++ console application that appears to be an initial or exploratory version of a heat simulation program. It features a menu-driven interface that provides access to different user roles (Admin, Viewer) and several data structures and algorithms that are not yet fully integrated.

The core heat diffusion logic is not implemented. The project currently consists of a collection of classes, some of which are stubs, representing concepts from different domains (simulation, data management, algorithms).

The project uses:
* **C++17**
* A command-line interface for user interaction.

---

## **2. Primary Goals (Inferred from Code)**

* Provide a menu system for navigating application features.
* Define user roles (`Admin`, `User`) with different permissions.
* Manage a `Grid` of temperature values.
* Store and manage a list of `HeatSource` objects using a `LinkedList`.
* Provide functionality for saving and loading the application state (`FileManager`).
* Include miscellaneous, currently unused algorithms for sorting and graph traversal.

---

## **3. Project Structure**

All source files are located in the `Code/` directory.

```
DSA_project/
│
├── /Code
│   ├── Admin.h
│   ├── FileManager.h
│   ├── GraphTraversal.h
│   ├── Grid.cpp
│   ├── Grid.h
│   ├── HeatSource.cpp
│   ├── HeatSource.h
│   ├── LinkedList.cpp
│   ├── LinkedList.h
│   ├── Main.cpp
│   ├── Menu.h
│   ├── Sorting.cpp
│   ├── Sorting.h
│   └── User.h
│
├── /Data
└── /Docs
    └── Project.MD
```

---

## **4. Module Responsibilities**

### **4.1 Core Components**

*   **`Main.cpp`**: The entry point of the application. It initializes and runs the main menu.
*   **`Menu.h`**: Provides static functions (`mainMenu`, `adminMenu`, `viewerMenu`) that create the text-based user interface.

### **4.2 Simulation & Data Structures**

*   **`Grid`**: Represents a 2D grid of `double` values, intended to store temperatures. The `update()` method, which would contain the diffusion logic, is not implemented.
*   **`HeatSource`**: A simple class holding the coordinates (`x`, `y`) and `intensity` of a heat source.
*   **`LinkedList`**: A singly linked list designed to store `HeatSource` objects. The core methods (`addSource`, `removeSource`) are currently stubs.

### **4.3 User Management**

*   **`User.h`**: An abstract base class defining a common interface with a virtual `viewHeatmap()` method.
*   **`Admin.h`**: A concrete class inheriting from `User`. It holds references to the `Grid` and the `LinkedList` of heat sources, suggesting it is intended to modify the simulation state.

### **4.4 Utility and Algorithm Modules**

*   **`FileManager.h`**: Declares static methods to `saveState` and `loadState`. The implementation is not provided.
*   **`GraphTraversal.h`**: Declares static methods for Breadth-First Search (`bfs`), Depth-First Search (`dfs`), and a `recursivePropagation` function on the grid. Their purpose in the context of the simulation is unclear.
*   **`Sorting.h`**: Declares various sorting algorithms (`bubbleSort`, `insertionSort`, etc.). These are not used anywhere in the project and their implementations are empty.

---

## **5. Current Execution Flow**

1.  `main()` is called.
2.  `Menu::mainMenu()` is invoked, which likely presents the user with initial choices.
3.  The program waits for user input to navigate to other menus (Admin or Viewer).
4.  Functionality beyond menu navigation is largely unimplemented.

---

## **6. AI Agent Behavioral Guidelines**

When modifying the code:

1.  **Acknowledge the Discrepancy:** The current code does not match a standard heat diffusion model. Clarify with the user whether the goal is to build the simulation as described previously or to complete the currently implemented features.
2.  **Complete Stubbed Functions:** Many functions are empty. Work on implementing them based on user requests.
3.  **Integrate Modules:** The various modules (`Sorting`, `GraphTraversal`, `User` management) are not connected. If requested, find a purpose for them or remove them to simplify the project.
4.  **Follow Existing Style:** Adhere to the coding style found in the existing files (e.g., `PascalCase` for classes, `camelCase` for variables if that's the pattern).

---

**End of Project Specification Document**
