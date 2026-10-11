#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

// ============================================================================
// DS/PL CO3: Generic Queue (FIFO)
// Application: Normal Outpatient Appointment Scheduling
// ============================================================================
template <typename T>
class Queue {
private:
    std::vector<T> elements;

public:
    Queue() {}

    void enqueue(const T& item) {
        elements.push_back(item);
    }

    bool dequeue(T& item) {
        if (isEmpty()) return false;
        item = elements.front();
        elements.erase(elements.begin());
        return true;
    }

    T dequeue() {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        T item = elements.front();
        elements.erase(elements.begin());
        return item;
    }

    const T& peek() const {
        if (isEmpty()) throw std::underflow_error("Queue is empty");
        return elements.front();
    }

    bool isEmpty() const { return elements.empty(); }
    size_t size() const { return elements.size(); }
    void clear() { elements.clear(); }

    const std::vector<T>& getElements() const { return elements; }

    void display() const {
        std::cout << "[Queue (FIFO, Size " << elements.size() << ")]: ";
        for (size_t i = 0; i < elements.size(); ++i) {
            std::cout << elements[i] << (i + 1 < elements.size() ? " -> " : "");
        }
        std::cout << "\n";
    }
};

// ============================================================================
// DS/PL CO3: Priority Queue (Heap Implementation)
// Application: Emergency Patient Prioritization (1 = Highest, 2 = High, 3 = Normal)
// ============================================================================
template <typename T>
class PriorityQueue {
public:
    struct Item {
        T data;
        int priority; // Lower numeric value = Higher priority (Min-heap)

        bool operator>(const Item& other) const {
            return priority > other.priority;
        }
        bool operator<(const Item& other) const {
            return priority < other.priority;
        }
    };

private:
    std::vector<Item> heap;

    void heapifyUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) { // Min-heap property
                std::swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(size_t index) {
        size_t n = heap.size();
        while (true) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t smallest = index;

            if (left < n && heap[left] < heap[smallest]) {
                smallest = left;
            }
            if (right < n && heap[right] < heap[smallest]) {
                smallest = right;
            }

            if (smallest != index) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    PriorityQueue() {}

    void enqueue(const T& data, int priority) {
        heap.push_back({data, priority});
        heapifyUp(heap.size() - 1);
    }

    bool dequeue(T& data, int& priority) {
        if (isEmpty()) return false;
        data = heap[0].data;
        priority = heap[0].priority;
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return true;
    }

    T dequeue() {
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        T data = heap[0].data;
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return data;
    }

    const Item& peek() const {
        if (isEmpty()) throw std::underflow_error("PriorityQueue is empty");
        return heap[0];
    }

    bool isEmpty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
    void clear() { heap.clear(); }

    const std::vector<Item>& getItems() const { return heap; }

    void display() const {
        std::cout << "[PriorityQueue (Emergency Min-Heap, Size " << heap.size() << ")]:\n";
        if (heap.empty()) {
            std::cout << "  (Empty)\n";
            return;
        }
        for (size_t i = 0; i < heap.size(); ++i) {
            std::cout << "  [Pos " << i << "] Priority " << heap[i].priority 
                      << ": " << heap[i].data << "\n";
        }
    }
};

#endif // QUEUE_H
