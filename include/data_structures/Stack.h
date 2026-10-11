#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <vector>
#include <stdexcept>

// ============================================================================
// DS/PL CO3: Generic Stack with Capacity Control (LIFO)
// Application: Hospital Operation History (Undo / Audit Trail)
// ============================================================================
template <typename T>
class Stack {
private:
    std::vector<T> elements;
    size_t maxCapacity;

public:
    explicit Stack(size_t capacity = 50) : maxCapacity(capacity) {}

    bool push(const T& item) {
        if (elements.size() >= maxCapacity) {
            // Drop oldest operation to maintain history window
            elements.erase(elements.begin());
        }
        elements.push_back(item);
        return true;
    }

    bool pop(T& item) {
        if (isEmpty()) return false;
        item = elements.back();
        elements.pop_back();
        return true;
    }

    T pop() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        T item = elements.back();
        elements.pop_back();
        return item;
    }

    const T& peek() const {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return elements.back();
    }

    bool isEmpty() const { return elements.empty(); }
    bool isFull() const { return elements.size() >= maxCapacity; }
    size_t size() const { return elements.size(); }
    size_t capacity() const { return maxCapacity; }
    void clear() { elements.clear(); }

    const std::vector<T>& getAllElements() const { return elements; }

    void display() const {
        std::cout << "[Stack (LIFO History, Size " << elements.size() << "/" << maxCapacity << ")]:\n";
        if (elements.empty()) {
            std::cout << "  (Empty)\n";
            return;
        }
        for (int i = static_cast<int>(elements.size()) - 1; i >= 0; --i) {
            std::cout << "  [" << i << "]: " << elements[i] << "\n";
        }
    }
};

#endif // STACK_H
