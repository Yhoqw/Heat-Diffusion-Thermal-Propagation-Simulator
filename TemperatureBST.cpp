#include "TemperatureBST.h"
#include "Grid.h" 
#include <stack>

BSTNode* TemperatureBST::insert(BSTNode* node, float temp) {
    if (node == nullptr)
        return new BSTNode(temp);

    if (temp < node->temp)
        node->left = insert(node->left, temp);
    else
        node->right = insert(node->right, temp);

    return node;
}

bool TemperatureBST::search(BSTNode* node, float temp) {
    if (!node) return false;
    if (node->temp == temp) return true;
    if (temp < node->temp) return search(node->left, temp);
    return search(node->right, temp);
}

void TemperatureBST::inorder(BSTNode* node, std::vector<float>& list) {
    if (!node) return;
    inorder(node->left, list);
    list.push_back(node->temp);
    inorder(node->right, list);
}

float TemperatureBST::findMin(BSTNode* node) {
    while (node->left)
        node = node->left;
    return node->temp;
}

float TemperatureBST::findMax(BSTNode* node) {
    while (node->right)
        node = node->right;
    return node->temp;
}

void TemperatureBST::insert(float temp) {
    root = insert(root, temp);
}

bool TemperatureBST::search(float temp) {
    return search(root, temp);
}

float TemperatureBST::getMin() {
    if (!root) return 0.0f;
    return findMin(root);
}

float TemperatureBST::getMax() {
    if (!root) return 0.0f;
    return findMax(root);
}

std::vector<float> TemperatureBST::getSortedList() {
    std::vector<float> list;
    inorder(root, list);
    return list;
}

void TemperatureBST::clear() {
    root = nullptr;  // no full delete for simplicity
}

void updateBST(const Grid& grid, TemperatureBST& bst) {
    bst.clear();

    for (int y = 0; y < grid.get_rows(); y++) {
        for (int x = 0; x < grid.get_cols(); x++) {
            bst.insert(grid.get_temperature(x, y));
        }
    }
}