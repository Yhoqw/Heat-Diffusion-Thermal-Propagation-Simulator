#pragma once
#include <vector>

class Grid; 

struct BSTNode {
    float temp;
    BSTNode* left;
    BSTNode* right;

    BSTNode(float t) : temp(t), left(nullptr), right(nullptr) {}
};

class TemperatureBST {
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, float temp);
    bool search(BSTNode* node, float temp);
    void inorder(BSTNode* node, std::vector<float>& list);
    float findMin(BSTNode* node);
    float findMax(BSTNode* node);

public:
    TemperatureBST() : root(nullptr) {}

    void insert(float temp);
    bool search(float temp);

    //getters
    float getMin();
    float getMax();
    std::vector<float> getSortedList();
    void clear();
};

void updateBST(const Grid& grid, TemperatureBST& bst);