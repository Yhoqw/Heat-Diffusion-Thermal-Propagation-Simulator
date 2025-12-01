//---------------------------------------Raylib Header----------------------------------------------
#if __has_include("raylib.h")
    #include "raylib.h"
#else
    #include "C:\\raylib\\raylib\\src\\raylib.h"
#endif
//---|g++ -o Heatsim.exe Heatsim.cpp -I. -IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm|---
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <queue>
#include <stack>

/*  TODO:
    Heat Source Management UI — click to add, right-click to remove, list display.
    Hash map for fast cell lookup — O(1) existence checks, marking visited.
    Sorting engine — implement Bubble / Insertion / Merge / Quick sorts to rank cells by temperature.
    Tree (Quadtree/BST) — spatial queries / top-k region queries.
    File saving/loading — persist heat sources & grid states.
    UI enhancements — sliders for alpha/cooling, highlighting hottest/coldest, graphs.
    Performance optimizations — quadtree partitioning, multi-threading (optional).
*/

enum SimulationMode {
    MODE_NORMAL,
    MODE_BFS,
    MODE_DFS
};

// Essentially a different name for Node used in most data structures
struct HeatSource {
    int x;
    int y;
    float temperature;
    HeatSource* next;
    HeatSource(int x_, int y_, float temp_) : x(x_), y(y_), temperature(temp_), next(nullptr) {}
};

//LinkedList for Heat sources
class HeatSourceList {
private:
    HeatSource* head;

public:
    HeatSourceList() : head(nullptr) {}

    void add_source(int x, int y, float temp) {
        HeatSource* node = new HeatSource(x,y,temp);
        node->next = head;
        head = node;
    }

    bool remove_source_at(int x, int y){
        HeatSource* cur = head;
        HeatSource* prev = nullptr;

        while (cur) {
            if (cur->x == x && cur->y == y) {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    void update_source_temperature(int x, int y, float temp) {
        HeatSource* cur = head;
        while (cur) {
            if (cur->x == x && cur->y == y) {
                cur->temperature = temp;
                return;
            }
            cur = cur->next;
        }
        add_source(x, y, temp);
    }

    // Traverse: apply a function to each node
    template<typename Fn>
    void for_each(Fn fn) const {
        HeatSource* cur = head;
        while (cur) {
            fn(cur);
            cur = cur->next;
        }
    }
    
    bool exists_at(int x, int y) const {
        HeatSource* cur = head;
        while (cur) {
            if (cur->x == x && cur->y == y) return true;
            cur = cur->next;
        }
        return false;
    }

    ~HeatSourceList() {
        HeatSource* cur = head;
        while (cur) {
            HeatSource* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }
};

/* Grid Class
    Stores a 2D vector grid[y][x] representing temperatures.

Supports:
    Adding heat at any cell

    Simulating diffusion using the formula:
    T_new = T_old + α(N_avg - T_old)

    Cooling factor applied uniformly every step
*/
class Grid {
private:
    int rows;
    int cols;
    float alpha;
    float cooling;
    std::vector<std::vector<float>> grid;

public:
    SimulationMode mode = MODE_NORMAL;
    HeatSourceList sources;

    Grid(int r, int c, float a = 0.1f, float cool = 0.7f) : rows(r), cols(c), alpha(a), cooling(cool) {
        grid.resize(rows, std::vector<float>(cols, 0.0f));
    }

    void add_heat_direct(int x, int y, float temperature) {
        if (x >= 0 && x < cols && y >= 0 && y < rows) {
            grid[y][x] = temperature;
        }
    }

    void inject_sources() {
        sources.for_each([this](const HeatSource* s){
            if (s->x >= 0 && s->x < cols && s->y >= 0 && s->y < rows) {
                // We add the source temperature (max) — you can blend instead
                grid[s->y][s->x] = std::max(grid[s->y][s->x], s->temperature);
            }
        });        
    }

    void step() {
        if (mode == MODE_NORMAL) {
            inject_sources();

            std::vector<std::vector<float>> new_grid = grid;

            for (int i = 1; i < rows - 1; i++) {
                for (int j = 1; j < cols - 1; j++) {
                    float avg_neighbor = (grid[i-1][j] + grid[i+1][j] +
                                        grid[i][j-1] + grid[i][j+1]) / 4.0f;

                    new_grid[i][j] += alpha * (avg_neighbor - grid[i][j]);
                }
            }

            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    new_grid[i][j] *= cooling;

            grid = new_grid;
        }
        else if (mode == MODE_BFS) {
            step_bfs();
        }
        else if (mode == MODE_DFS) {
            step_dfs();
        }
    }

    float get_temperature(int x, int y) const {
        if (x >= 0 && x < cols && y >= 0 && y < rows) {
            return grid[y][x];
        }
        return 0.0f;
    }

    //BFS and DFS
    void step_bfs() {
        inject_sources();

        std::vector<std::vector<float>> new_grid = grid;
        std::queue<std::pair<int,int>> q;

        // enqueue all heat sources
        sources.for_each([&](const HeatSource* s){
            q.push({s->x, s->y});
        });

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();

            float current_temp = grid[y][x];
            float spread_temp = current_temp * alpha;

            if (spread_temp < 0.1f) continue; // threshold

            const int dx[4] = {1, -1, 0, 0};
            const int dy[4] = {0, 0, 1, -1};

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                    if (spread_temp > new_grid[ny][nx]) {
                        new_grid[ny][nx] = spread_temp;
                        q.push({nx, ny});
                    }
                }
            }
        }

        grid = new_grid;
    }

    void step_dfs() {
        inject_sources();

        std::vector<std::vector<float>> new_grid = grid;
        std::stack<std::pair<int,int>> st;

        // push all heat sources
        sources.for_each([&](const HeatSource* s){
            st.push({s->x, s->y});
        });

        while (!st.empty()) {
            auto [x, y] = st.top();
            st.pop();

            float current_temp = grid[y][x];
            float spread_temp = current_temp * alpha;

            if (spread_temp < 0.1f) continue;

            const int dx[4] = {1, -1, 0, 0};
            const int dy[4] = {0, 0, 1, -1};

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                    if (spread_temp > new_grid[ny][nx]) {
                        new_grid[ny][nx] = spread_temp;
                        st.push({nx, ny});
                    }
                }
            }
        }

        grid = new_grid;
    }

    std::vector<float> get_all_temperatures() const {
        std::vector<float> temps;
        temps.reserve(rows * cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                temps.push_back(grid[i][j]);
            }
        }
        return temps;
    }

    int get_rows() const { return rows; }
    int get_cols() const { return cols; }
};

//Sorting Algorithms
void selectionSort(std::vector<float>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIndex])
                minIndex = j;
        std::swap(arr[i], arr[minIndex]);
    }
}

void bubbleSort(std::vector<float>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) return;
    }
}

void insertionSort(std::vector<float>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        float key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(std::vector<float>& arr, int left, int mid, int right) {
    std::vector<float> L(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<float> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < L.size() && j < R.size())
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < L.size()) arr[k++] = L[i++];
    while (j < R.size()) arr[k++] = R[j++];
}

void mergeSort(std::vector<float>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int partition(std::vector<float>& arr, int low, int high) {
    float pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<float>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Trees
struct BSTNode {
    float temp;
    BSTNode* left;
    BSTNode* right;

    BSTNode(float t) : temp(t), left(nullptr), right(nullptr) {}
};

class TemperatureBST {
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, float temp) {
        if (node == nullptr)
            return new BSTNode(temp);

        if (temp < node->temp)
            node->left = insert(node->left, temp);
        else
            node->right = insert(node->right, temp);

        return node;
    }

    bool search(BSTNode* node, float temp) {
        if (!node) return false;
        if (node->temp == temp) return true;
        if (temp < node->temp) return search(node->left, temp);
        return search(node->right, temp);
    }

    void inorder(BSTNode* node, std::vector<float>& list) {
        if (!node) return;
        inorder(node->left, list);
        list.push_back(node->temp);
        inorder(node->right, list);
    }

    float findMin(BSTNode* node) {
        while (node->left)
            node = node->left;
        return node->temp;
    }

    float findMax(BSTNode* node) {
        while (node->right)
            node = node->right;
        return node->temp;
    }

public:
    TemperatureBST() : root(nullptr) {}

    void insert(float temp) {
        root = insert(root, temp);
    }

    bool search(float temp) {
        return search(root, temp);
    }

    float getMin() {
        if (!root) return 0.0f;
        return findMin(root);
    }

    float getMax() {
        if (!root) return 0.0f;
        return findMax(root);
    }

    std::vector<float> getSortedList() {
        std::vector<float> list;
        inorder(root, list);
        return list;
    }

    void clear() {
        root = nullptr;  // no full delete for simplicity
    }
};

void updateBST(const Grid& grid, TemperatureBST& bst) {
    bst.clear();

    for (int y = 0; y < grid.get_rows(); y++) {
        for (int x = 0; x < grid.get_cols(); x++) {
            bst.insert(grid.get_temperature(x, y));
        }
    }
}


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
        DrawText(TextFormat("Alpha: %.2f | Cooling: %.3f", 0.3f, 0.999f),
                 10, SCREEN_HEIGHT - 22, 14, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}