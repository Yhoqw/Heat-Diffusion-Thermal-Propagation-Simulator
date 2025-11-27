#pragma once
#include <vector>

class Sorting {
public:
    static void bubbleSort(std::vector<double>& arr);
    static void insertionSort(std::vector<double>& arr);
    static void mergeSort(std::vector<double>& arr, int l, int r);
    static void quickSort(std::vector<double>& arr, int l, int r);
};