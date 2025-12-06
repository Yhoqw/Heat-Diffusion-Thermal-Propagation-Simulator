#include "HeatSourcelist.h"

HeatSourceList::HeatSourceList() : head(nullptr) {}

HeatSourceList::~HeatSourceList() {
    HeatSource* cur = head;
    while (cur) {
        HeatSource* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

void HeatSourceList::add_source(int x, int y, float temp) {
    HeatSource* node = new HeatSource(x,y,temp);
    node->next = head;
    head = node;
}

bool HeatSourceList::remove_source_at(int x, int y){
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

void HeatSourceList::update_source_temperature(int x, int y, float temp) {
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

bool HeatSourceList::exists_at(int x, int y) const {
    HeatSource* cur = head;
    while (cur) {
        if (cur->x == x && cur->y == y) return true;
        cur = cur->next;
    }
    return false;
}