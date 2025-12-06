//---------------------------------------Raylib Header----------------------------------------------
#if __has_include("raylib.h")
    #include "raylib.h"
#else
    #include "C:\\raylib\\raylib\\src\\raylib.h"
#endif
//g++ -o Code/Heatsim.exe Code/Heatsim.cpp Code/Grid.cpp Code/HeatSourcelist.cpp Code/Sorting_Algorithms.cpp Code/TemperatureBST.cpp -I. -IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm |input this code into terminal to run code

#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <queue>
#include <stack>

#include "Sorting_Algorithms.h"
#include "HeatSourcelist.h" // HeatSource struct
#include "Grid.h" // Grid class
#include "TemperatureBST.h" // TemperatureBST class

//========================================================|

// Convert temperature to color (hot colormap)
Color temperature_to_color(float temp) {
    // Clamp temperature to 0-100 range
    temp = std::max(0.0f, std::min(100.0f, temp));
    float t = temp / 100.0f;
    
    unsigned char r, g, b;
    
    // Hot colormap approximation
    if (t < 0.33f) {
        // Black to red
        r = (unsigned char)(255 * (t / 0.33f));
        g = 0;
        b = 0;
    } else if (t < 0.66f) {
        // Red to yellow
        r = 255;
        g = (unsigned char)(255 * ((t - 0.33f) / 0.33f));
        b = 0;
    } else {
        // Yellow to white
        r = 255;
        g = 255;
        b = (unsigned char)(255 * ((t - 0.66f) / 0.34f));
    }
    
    return (Color){r, g, b, 255};
}

int main() {
    const int GRID_ROWS = 16;
    const int GRID_COLS = 16;
    const int CELL_SIZE = 40;
    const int SCREEN_WIDTH = GRID_COLS * CELL_SIZE;
    const int SCREEN_HEIGHT = GRID_ROWS * CELL_SIZE + 80;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Heat Diffusion Simulation - LinkedList Sources");
    SetTargetFPS(60);

    Grid grid(GRID_ROWS, GRID_COLS, 0.3f, 0.999f);
    TemperatureBST bst;

    // Pre-add an example source
    // grid.sources.add_source(4, 4, 100.0f);

    while (!WindowShouldClose()) {
        // Input handling

        if (IsKeyPressed(KEY_ONE))  grid.mode = MODE_NORMAL;
        if (IsKeyPressed(KEY_TWO))  grid.mode = MODE_BFS;
        if (IsKeyPressed(KEY_THREE)) grid.mode = MODE_DFS;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            int gridX = (int)(mousePos.x / CELL_SIZE);
            int gridY = (int)(mousePos.y / CELL_SIZE);
            if (gridX >= 0 && gridX < GRID_COLS && gridY >= 0 && gridY < GRID_ROWS) {
                // Add source at clicked cell (if not already present)
                if (!grid.sources.exists_at(gridX, gridY)) {
                    grid.sources.add_source(gridX, gridY, 100.0f);
                } else {
                    // If exists, bump temperature
                    grid.sources.update_source_temperature(gridX, gridY, 100.0f);
                }
            }
        }

        // Right-click to remove a source at the clicked cell
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            int gridX = (int)(mousePos.x / CELL_SIZE);
            int gridY = (int)(mousePos.y / CELL_SIZE);
            if (gridX >= 0 && gridX < GRID_COLS && gridY >= 0 && gridY < GRID_ROWS) {
                grid.sources.remove_source_at(gridX, gridY);
            }
        }

        // Update simulation
        grid.step();
        updateBST(grid, bst);

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        const char* modeText =
            (grid.mode == MODE_NORMAL) ? "Mode: NORMAL Diffusion" :
            (grid.mode == MODE_BFS)    ? "Mode: BFS (Queue)" :
                                        "Mode: DFS (Stack)";

        DrawText(modeText, 10, SCREEN_HEIGHT - 60, 18, YELLOW);

        // Draw grid cells
        for (int y = 0; y < GRID_ROWS; y++) {
            for (int x = 0; x < GRID_COLS; x++) {
                float temp = grid.get_temperature(x, y);
                Color color = temperature_to_color(temp);
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE,
                              CELL_SIZE - 1, CELL_SIZE - 1, color);
            }
        }

        float minTemp = bst.getMin();
        float maxTemp = bst.getMax();

        DrawText(TextFormat("Min: %.2f", minTemp), 10, SCREEN_HEIGHT - 80, 20, LIGHTGRAY);
        DrawText(TextFormat("Max: %.2f", maxTemp), 200, SCREEN_HEIGHT - 80, 20, LIGHTGRAY);        

        // Draw heat source markers (outline small white rectangle)
        grid.sources.for_each([&](const HeatSource* s){
            // draw a border to indicate source
            DrawRectangleLines(s->x * CELL_SIZE, s->y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, WHITE);
            // small text of temp
            DrawText(TextFormat("%.0f", s->temperature), s->x * CELL_SIZE + 4, s->y * CELL_SIZE + 4, 12, BLACK);
        });

        // UI text
        DrawText("Left click: add/upsert source | Right click: remove source", 10, SCREEN_HEIGHT - 70, 16, WHITE);
        DrawText("Sources are stored in a Linked List (DSA demo).", 10, SCREEN_HEIGHT - 44, 14, LIGHTGRAY);
        DrawText(TextFormat("Alpha: %.2f | Cooling: %.3f", 0.3f, 0.7f),
                 10, SCREEN_HEIGHT - 22, 14, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}