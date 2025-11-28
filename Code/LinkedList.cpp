#include "LinkedList.h"
#include <iostream>
#include <iomanip>

Node::Node(HeatSource src) : data(src), next(nullptr) {}

LinkedList::LinkedList() : head(nullptr) {}

LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void LinkedList::addSource(HeatSource src) {
    Node* newNode = new Node(src);
    newNode->next = head;
    head = newNode;
}

void LinkedList::removeSource(int x, int y) {
    // Handle removal of head node
    if (head != nullptr && head->data.x == x && head->data.y == y) {
        Node* temp = head;
        head = head->next;
        delete temp;
        std::cout << "Heat source at (" << x << ", " << y << ") removed." << std::endl;
        return;
    }

    // Handle removal of other nodes
    Node* current = head;
    while (current != nullptr && current->next != nullptr) {
        if (current->next->data.x == x && current->next->data.y == y) {
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
            std::cout << "Heat source at (" << x << ", " << y << ") removed." << std::endl;
            return;
        }
        current = current->next;
    }

    std::cout << "Heat source at (" << x << ", " << y << ") not found." << std::endl;
}

void LinkedList::display() const {
    if (head == nullptr) {
        std::cout << "No heat sources in the system." << std::endl;
        return;
    }

    std::cout << "\n=== Heat Sources List ===" << std::endl;
    std::cout << std::left << std::setw(10) << "Position" 
              << std::setw(15) << "Intensity" << std::endl;
    std::cout << std::string(25, '-') << std::endl;

    Node* current = head;
    int count = 0;
    while (current != nullptr) {
        std::cout << std::left << std::setw(10) 
                  << "(" << current->data.x << ", " << current->data.y << ")"
                  << std::setw(15) << current->data.intensity << std::endl;
        current = current->next;
        count++;
    }
    std::cout << std::string(25, '-') << std::endl;
    std::cout << "Total heat sources: " << count << std::endl << std::endl;
}

Node* LinkedList::getHead() const {
    return head;
}

int LinkedList::getCount() const {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}