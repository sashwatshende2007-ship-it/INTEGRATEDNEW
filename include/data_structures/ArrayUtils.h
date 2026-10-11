#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

// ============================================================================
// DS/PL CO1: FixedArray Template for Fixed Ward Bed Slots
// ============================================================================
template <typename T, size_t SIZE>
class FixedArray {
private:
    T data[SIZE];
    size_t count;

public:
    FixedArray() : count(0) {}

    bool insert(const T& value) {
        if (count >= SIZE) return false;
        data[count++] = value;
        return true;
    }

    bool removeAt(size_t index) {
        if (index >= count) return false;
        for (size_t i = index; i < count - 1; ++i) {
            data[i] = data[i + 1];
        }
        --count;
        return true;
    }

    int find(const T& value) const {
        for (size_t i = 0; i < count; ++i) {
            if (data[i] == value) return static_cast<int>(i);
        }
        return -1;
    }

    T& at(size_t index) {
        if (index >= count) throw std::out_of_range("FixedArray index out of range");
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= count) throw std::out_of_range("FixedArray index out of range");
        return data[index];
    }

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    size_t size() const { return count; }
    size_t capacity() const { return SIZE; }
    bool isFull() const { return count >= SIZE; }
    bool isEmpty() const { return count == 0; }
    void clear() { count = 0; }

    void display() const {
        std::cout << "[FixedArray (" << count << "/" << SIZE << ")]: ";
        for (size_t i = 0; i < count; ++i) {
            std::cout << data[i] << (i + 1 < count ? ", " : "");
        }
        std::cout << "\n";
    }
};

// ============================================================================
// DS/PL CO4: Searching Algorithms
// ============================================================================
namespace Searching {

    // Linear Search - O(n)
    template <typename T>
    int linearSearch(const std::vector<T>& arr, const T& target) {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i] == target) return static_cast<int>(i);
        }
        return -1;
    }

    // Binary Search - O(log n) (Requires sorted vector)
    template <typename T>
    int binarySearch(const std::vector<T>& arr, const T& target) {
        int left = 0;
        int right = static_cast<int>(arr.size()) - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid] == target) return mid;
            if (arr[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }

} // namespace Searching

// ============================================================================
// DS/PL CO4: Sorting Algorithms
// ============================================================================
namespace Sorting {

    // Bubble Sort - O(n^2)
    template <typename T>
    void bubbleSort(std::vector<T>& arr) {
        size_t n = arr.size();
        for (size_t i = 0; i < n; ++i) {
            bool swapped = false;
            for (size_t j = 0; j + 1 < n - i; ++j) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    // Selection Sort - O(n^2)
    template <typename T>
    void selectionSort(std::vector<T>& arr) {
        size_t n = arr.size();
        for (size_t i = 0; i < n; ++i) {
            size_t minIdx = i;
            for (size_t j = i + 1; j < n; ++j) {
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }
            if (minIdx != i) std::swap(arr[i], arr[minIdx]);
        }
    }

    // Insertion Sort - O(n^2)
    template <typename T>
    void insertionSort(std::vector<T>& arr) {
        size_t n = arr.size();
        for (size_t i = 1; i < n; ++i) {
            T key = arr[i];
            int j = static_cast<int>(i) - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            }
            arr[j + 1] = key;
        }
    }

    // Merge Sort Helper
    template <typename T>
    void merge(std::vector<T>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        std::vector<T> L(n1), R(n2);
        for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
        for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    template <typename T>
    void mergeSortInternal(std::vector<T>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortInternal(arr, left, mid);
            mergeSortInternal(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

    // Merge Sort - O(n log n)
    template <typename T>
    void mergeSort(std::vector<T>& arr) {
        if (!arr.empty()) {
            mergeSortInternal(arr, 0, static_cast<int>(arr.size()) - 1);
        }
    }

    // Quick Sort Helper
    template <typename T>
    int partition(std::vector<T>& arr, int low, int high) {
        T pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (arr[j] <= pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    template <typename T>
    void quickSortInternal(std::vector<T>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortInternal(arr, low, pi - 1);
            quickSortInternal(arr, pi + 1, high);
        }
    }

    // Quick Sort - O(n log n)
    template <typename T>
    void quickSort(std::vector<T>& arr) {
        if (!arr.empty()) {
            quickSortInternal(arr, 0, static_cast<int>(arr.size()) - 1);
        }
    }

} // namespace Sorting

#endif // ARRAY_UTILS_H
