#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "State.h"

class HashTable {
private:
    struct HashNode {
        State* state;
        HashNode* next;
        HashNode(State* s) : state(s), next(nullptr) {}
    };

    HashNode** table;
    int capacity;
    int size;
    int totalInsertions;
    double loadFactorThreshold;
    int rehashCount;
    int maxChainLength;
    int collisionCount;

    int hashFunction(State* state);
    void rehash();

public:
    HashTable(int initialCapacity = 1000, double loadFactorThreshold = 0.75);
    ~HashTable();
    void insert(State* state);
    bool contains(State* state);
    
    // getters
    int getSize() const { return size; }
    double getLoadFactorThreshold() const { return loadFactorThreshold; }
    int getCapacity() const { return capacity; }
    int getTotalInsertions() const { return totalInsertions; }
    int getRehashCount() const { return rehashCount; }
    int getMaxChainLength() const { return maxChainLength; }
    int getCollisionCount() const { return collisionCount; }
    double getLoadFactor() const { return (double)size / capacity; }
    
    void clear();
    void printStats() const;
    void printDetailedStats() const;
};

#endif