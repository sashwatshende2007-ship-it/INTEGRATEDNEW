#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>

// ============================================================================
// DS/PL CO5: Hash Table with Open Addressing & Linear Probing
// Application: Fast O(1) Patient ID Lookup in Hospital Database
// ============================================================================
template <typename Key, typename Value>
class HashTable {
public:
    enum EntryState { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        Key key;
        Value value;
        EntryState state;

        Entry() : state(EMPTY) {}
        Entry(const Key& k, const Value& v) : key(k), value(v), state(OCCUPIED) {}
    };

private:
    std::vector<Entry> table;
    size_t count;
    size_t capacity;
    double maxLoadFactor;
    std::hash<Key> hasher;

    size_t hashFunction(const Key& key) const {
        return hasher(key) % capacity;
    }

    void rehash() {
        size_t oldCapacity = capacity;
        std::vector<Entry> oldTable = table;

        capacity = oldCapacity * 2 + 1;
        table.clear();
        table.resize(capacity);
        count = 0;

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].state == OCCUPIED) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

public:
    explicit HashTable(size_t initCap = 17, double loadFactor = 0.7)
        : count(0), capacity(initCap), maxLoadFactor(loadFactor) {
        table.resize(capacity);
    }

    bool insert(const Key& key, const Value& value) {
        if (static_cast<double>(count + 1) / capacity > maxLoadFactor) {
            rehash();
        }

        size_t index = hashFunction(key);
        size_t start = index;
        int firstDeleted = -1;

        while (table[index].state != EMPTY) {
            if (table[index].state == OCCUPIED && table[index].key == key) {
                // Key already exists, update value
                table[index].value = value;
                return true;
            }
            if (table[index].state == DELETED && firstDeleted == -1) {
                firstDeleted = static_cast<int>(index);
            }
            index = (index + 1) % capacity;
            if (index == start) break; // Wrapped around
        }

        // Use deleted slot if found, else vacant empty slot
        size_t targetIndex = (firstDeleted != -1) ? static_cast<size_t>(firstDeleted) : index;
        table[targetIndex] = Entry(key, value);
        ++count;
        return true;
    }

    bool find(const Key& key, Value& value) const {
        if (capacity == 0) return false;
        size_t index = hashFunction(key);
        size_t start = index;

        while (table[index].state != EMPTY) {
            if (table[index].state == OCCUPIED && table[index].key == key) {
                value = table[index].value;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == start) break;
        }
        return false;
    }

    bool remove(const Key& key) {
        if (capacity == 0) return false;
        size_t index = hashFunction(key);
        size_t start = index;

        while (table[index].state != EMPTY) {
            if (table[index].state == OCCUPIED && table[index].key == key) {
                table[index].state = DELETED;
                --count;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == start) break;
        }
        return false;
    }

    bool contains(const Key& key) const {
        Value dummy;
        return find(key, dummy);
    }

    size_t size() const { return count; }
    size_t getCapacity() const { return capacity; }
    double currentLoadFactor() const { return static_cast<double>(count) / capacity; }

    void displayStats() const {
        std::cout << "[HashTable Statistics]:\n";
        std::cout << "  Items: " << count << " / Capacity: " << capacity 
                  << " (Load Factor: " << std::fixed << std::setprecision(2) << currentLoadFactor() << ")\n";
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].state == OCCUPIED) {
                std::cout << "  Slot [" << i << "]: Key=" << table[i].key << "\n";
            }
        }
    }
};

// ============================================================================
// Specialized Patient Record for Hash Table Lookup
// ============================================================================
struct PatientQuickInfo {
    std::string patientId;
    std::string name;
    int age;
    std::string condition;
    int bedId;
    bool isEmergency;

    PatientQuickInfo() : age(0), bedId(-1), isEmergency(false) {}
    PatientQuickInfo(std::string id, std::string n, int a, std::string c, int b, bool emg)
        : patientId(id), name(n), age(a), condition(c), bedId(b), isEmergency(emg) {}
};

class PatientHashTable {
private:
    HashTable<std::string, PatientQuickInfo> table;

public:
    PatientHashTable(size_t cap = 23) : table(cap) {}

    bool addPatient(const std::string& id, const std::string& name, int age,
                    const std::string& condition, int bedId, bool isEmergency) {
        PatientQuickInfo info(id, name, age, condition, bedId, isEmergency);
        return table.insert(id, info);
    }

    bool findPatient(const std::string& id, PatientQuickInfo& info) const {
        return table.find(id, info);
    }

    bool removePatient(const std::string& id) {
        return table.remove(id);
    }

    bool contains(const std::string& id) const {
        return table.contains(id);
    }

    size_t size() const { return table.size(); }
    void displayStats() const { table.displayStats(); }
};

#endif // HASH_TABLE_H
