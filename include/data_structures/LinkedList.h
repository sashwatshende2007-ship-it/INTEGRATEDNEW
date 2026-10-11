#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <stdexcept>
#include <functional>

// ============================================================================
// DS/PL CO2: Generic Doubly Linked List Template
// Application: Patient Medical Record History Chain
// ============================================================================
template <typename T>
class LinkedList {
public:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
    };

private:
    Node* head;
    Node* tail;
    size_t count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~LinkedList() {
        clear();
    }

    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* curr = other.head;
        while (curr) {
            pushBack(curr->data);
            curr = curr->next;
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* curr = other.head;
            while (curr) {
                pushBack(curr->data);
                curr = curr->next;
            }
        }
        return this;
    }

    void pushFront(const T& val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++count;
    }

    void pushBack(const T& val) {
        Node* newNode = new Node(val);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++count;
    }

    bool insertAt(size_t index, const T& val) {
        if (index > count) return false;
        if (index == 0) {
            pushFront(val);
            return true;
        }
        if (index == count) {
            pushBack(val);
            return true;
        }
        Node* curr = head;
        for (size_t i = 0; i < index; ++i) {
            curr = curr->next;
        }
        Node* newNode = new Node(val);
        newNode->prev = curr->prev;
        newNode->next = curr;
        curr->prev->next = newNode;
        curr->prev = newNode;
        ++count;
        return true;
    }

    bool removeAt(size_t index) {
        if (index >= count) return false;
        Node* toDelete = nullptr;
        if (index == 0) {
            toDelete = head;
            head = head->next;
            if (head) head->prev = nullptr;
            else tail = nullptr;
        } else if (index == count - 1) {
            toDelete = tail;
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;
        } else {
            toDelete = head;
            for (size_t i = 0; i < index; ++i) {
                toDelete = toDelete->next;
            }
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;
        }
        delete toDelete;
        --count;
        return true;
    }

    Node* getHead() const { return head; }
    Node* getTail() const { return tail; }
    size_t size() const { return count; }
    bool isEmpty() const { return count == 0; }

    void clear() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        head = tail = nullptr;
        count = 0;
    }

    void forEach(std::function<void(const T&)> func) const {
        Node* curr = head;
        while (curr) {
            func(curr->data);
            curr = curr->next;
        }
    }

    void display() const {
        std::cout << "[LinkedList (Size: " << count << ")]: ";
        Node* curr = head;
        while (curr) {
            std::cout << curr->data;
            if (curr->next) std::cout << " <-> ";
            curr = curr->next;
        }
        std::cout << "\n";
    }
};

#endif // LINKED_LIST_H
