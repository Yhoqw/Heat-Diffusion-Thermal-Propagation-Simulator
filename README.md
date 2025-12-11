Heat Diffusion & Thermal Propagation Simulator
A C++ and Raylib–based simulation of heat flow across a 2D grid, featuring multiple traversal algorithms, dynamic heat sources, sorting modules, and a temperature Binary Search Tree.



OVERVIEW:-
This project implements a real-time 2D heat diffusion simulator where heat spreads across a grid using three different computational approaches:

Normal Diffusion (physics-based averaging)

BFS Heat Spread (Queue-based)

DFS Heat Spread (Stack-based)

Users can add and remove heat sources using the mouse. Temperatures are stored and analyzed using a linked list, custom sorting algorithms, and a binary search tree.
The project demonstrates core Data Structures & Algorithms through a visually interactive simulation.



HOW SIMULATOR WORKS:-
1.The grid begins at temperature 0 everywhere.

2.Clicking adds a heat source at that cell:
Stored in a Linked List (HeatSourceList)
Automatically injected into the grid each simulation step

3.Heat spreads using the active mode:
Normal mode (numerical heat diffusion)
BFS mode (queue-based expansion)
DFS mode (stack-based expansion)

4.Every frame, all grid temperatures are inserted into a Binary Search Tree, which allows instant min/max queries and sorted traversals.



SOURCE FILE DOCUMENTATION:-



GRID.CPP:-
Implements the 2D heat grid, all simulation logic, and BFS/DFS heat propagation.

Features:

Stores temperatures in a 2D vector
Injects heat sources from a linked list
Normal diffusion physics:
new_grid[i][j] += alpha * (avg_neighbor - grid[i][j]);

Cooling factor applied each step
BFS mode uses queue
DFS mode uses stack

Exposes functions:
getTemperature
setCellTemperature
getTemperatureList (for sorting and BST)
applyHeatSources
update



HEATSOURCELIST.CPP:-
A singly linked list storing all heat sources.

Features:
Add a heat source (x, y, temperature)
Remove a heat source by coordinates
Update temperature of existing source
Iterate using for_each(callback)
Used by Grid to apply heat injections each frame

This file demonstrates pointer-based dynamic memory management.



HEATSIM.CPP(MAIN SIMULATION):-
Runs the Raylib visualization, handles keyboard and mouse input, and integrates all modules.

Includes:
Grid simulation engine
HeatSourceList management
TemperatureBST updates for analytics
Optional sorting using Sorting_Algorithms

User Actions:
Left-click: Add or amplify heat source
Right-click: Remove heat source
Key 1: Switch to normal diffusion
Key 2: Switch to BFS mode
Key 3: Switch to DFS mode

Rendering:
Converts grid temperature into color gradients
Displays min/max temperature via BST
Outlines cells that contain heat sources



SORTING_ALGORITHMS.CPP:-
Contains manual implementations of classical sorting algorithms:

Selection Sort
Bubble Sort
Insertion Sort
Merge Sort
Quick Sort

Used for DSA demonstration and for optional temperature analysis.



TEMPERATUREBST.CPP:-
Implements a binary search tree for storing all grid temperatures every frame.

Used to compute:

Minimum temperature
Maximum temperature
A sorted list of all temperatures

Features:
Recursive insertion
Inorder traversal
Search functionality

Clear-and-rebuild operations each frame



ALGORITHMS USED:-
| Feature               | Data Structure / Algorithm    |
| --------------------- | ----------------------------- |
| Heat Sources          | Linked List                   |
| Temperature Analytics | Binary Search Tree            |
| Heat Propagation      | BFS (Queue), DFS (Stack)      |
| Normal Diffusion      | Numerical averaging           |
| Sorting               | Multiple classical algorithms |
| Visualization         | Raylib                        |



CONTROLS:-
| Input              | Action                        |
| ------------------ | ----------------------------- |
| Left Mouse         | Add or strengthen heat source |
| Right Mouse        | Remove heat source            |
| 1                  | Normal diffusion              |
| 2                  | BFS heat spread               |
| 3                  | DFS heat spread               |
| Esc / Close Window | Exit                          |



HOW TO BUILD:-
Example compilation command (Windows + Raylib):
g++ -o Heatsim.exe Heatsim.cpp Grid.cpp HeatSourceList.cpp \
Sorting_Algorithms.cpp TemperatureBST.cpp \
-I. -IC:\raylib\raylib\src -LC:\raylib\raylib\src \
-lraylib -lopengl32 -lgdi32 -lwinmm



FUTURE IMPROVEMENTS:-
Save/load simulation states
GUI controls for alpha, cooling factor, propagation mode
Support diagonal diffusion (8-neighbor model)
Use Matplotlib-C++ for heat graphs
Implement Dijkstra or A* heat flow
Parallelized grid update








