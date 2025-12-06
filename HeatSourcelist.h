#pragma once

// Essentially a different name for Node used in most data structures
struct HeatSource {
    int x;
    int y;
    float temperature;
    HeatSource* next;
    HeatSource(int x_, int y_, float temp_) : x(x_), y(y_), temperature(temp_), next(nullptr) {}
};

class HeatSourceList {
private:
    HeatSource* head;
public:
    HeatSourceList();
    ~HeatSourceList();

    void add_source(int x, int y, float temp);
    bool remove_source_at(int x, int y);
    void update_source_temperature(int x, int y, float temp);
    bool exists_at(int x, int y) const;

    template<typename Fn>   
    void for_each(Fn fn) const{
        HeatSource* cur = head;
        while (cur) {
            fn(cur);
            cur = cur->next;
        }
    }  
};