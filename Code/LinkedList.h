#pragma once
#include "HeatSource.h"

struct Node {
public:
    HeatSource data;
    Node* next;

    Node(HeatSource src);
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList();
    void addSource(HeatSource src);
    void removeSource(int x, int y);
    void display();
};