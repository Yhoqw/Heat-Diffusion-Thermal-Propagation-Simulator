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
    ~LinkedList();
    void addSource(HeatSource src);
    void removeSource(int x, int y);
    void display() const;
    Node* getHead() const;
    int getCount() const;
};