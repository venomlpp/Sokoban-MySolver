#include "HashTable.h"
#include <iostream>
#include <cmath>

using namespace std;

HashTable::HashTable(int initialCapacity, double loadFactorThreshold) 
    : capacity(initialCapacity), size(0), totalInsertions(0), 
      loadFactorThreshold(loadFactorThreshold), rehashCount(0), 
      maxChainLength(0), collisionCount(0) {
    table = new HashNode*[capacity]();
}

HashTable::~HashTable() {
    clear();
    delete[] table;
}

int HashTable::hashFunction(State* state) {
    // Función hash más robusta y mejor distribuida
    const int prime = 5381;
    long long hash = prime;
    
    // Hash de la posición del jugador
    hash = hash * 33 + state->x;
    hash = hash * 33 + state->y;
    hash = hash * 33 + state->energia;
    
    // Hash de las cajas (ordenar para que sea independiente del orden)
    for (int i = 0; i < state->numBoxes; i++) {
        hash = hash * 33 + state->boxX[i];
        hash = hash * 33 + state->boxY[i];
    }
    
    // Mezcla final
    hash = hash ^ (hash >> 16);
    return abs(static_cast<int>(hash)) % capacity;
}

void HashTable::rehash() {
    int oldCapacity = capacity;
    capacity = capacity * 2 + 1;
    HashNode** newTable = new HashNode*[capacity]();
    
    int newMaxChainLength = 0;
    
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            
            int newIndex = hashFunction(current->state);
            current->next = newTable[newIndex];
            newTable[newIndex] = current;
            
            int chainLength = 0;
            HashNode* temp = newTable[newIndex];
            while (temp != nullptr) {
                chainLength++;
                temp = temp->next;
            }
            if (chainLength > newMaxChainLength) {
                newMaxChainLength = chainLength;
            }
            
            current = next;
        }
    }
    
    delete[] table;
    table = newTable;
    maxChainLength = newMaxChainLength;
    rehashCount++;
    
    cout << "Rehashing #" << rehashCount << " realizado. Nueva capacidad: " << capacity << endl;
}

void HashTable::insert(State* state) {
    totalInsertions++;
    
    double currentLoadFactor = (double)size / capacity;
    if (currentLoadFactor >= loadFactorThreshold) {
        rehash();
    }
    
    int index = hashFunction(state);
    HashNode* newNode = new HashNode(state);
    
    if (table[index] != nullptr) {
        collisionCount++;
    }
    
    newNode->next = table[index];
    table[index] = newNode;
    size++;
    
    int chainLength = 0;
    HashNode* current = table[index];
    while (current != nullptr) {
        chainLength++;
        current = current->next;
    }
    if (chainLength > maxChainLength) {
        maxChainLength = chainLength;
    }
}

bool HashTable::contains(State* state) {
    int index = hashFunction(state);
    HashNode* current = table[index];
    
    while (current != nullptr) {
        if (current->state->equals(state)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void HashTable::clear() {
    for (int i = 0; i < capacity; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        table[i] = nullptr;
    }
    size = 0;
}

void HashTable::printStats() const {
    cout << "=== HashTable Statistics ===" << endl;
    cout << "Estados visitados (total insertions): " << totalInsertions << endl;
    cout << "Elementos actuales en tabla: " << size << endl;
    cout << "Capacidad actual: " << capacity << endl;
    cout << "Factor de carga: " << getLoadFactor() << " (Umbral: " << loadFactorThreshold << ")" << endl;
    cout << "Veces que se hizo rehashing: " << rehashCount << endl;
    cout << "Máxima longitud de cadena: " << maxChainLength << endl;
    cout << "Total de colisiones: " << collisionCount << endl;
}

void HashTable::printDetailedStats() const {
    printStats();
    
    int emptyBuckets = 0;
    int singleItemBuckets = 0;
    int longChains = 0; // Cadenas con más de 5 elementos
    double avgChainLength = 0;
    int totalNonEmptyBuckets = 0;
    
    for (int i = 0; i < capacity; i++) {
        int chainLength = 0;
        HashNode* current = table[i];
        while (current != nullptr) {
            chainLength++;
            current = current->next;
        }
        
        avgChainLength += chainLength;
        
        if (chainLength == 0) {
            emptyBuckets++;
        } else if (chainLength == 1) {
            singleItemBuckets++;
        }
        
        if (chainLength > 5) {
            longChains++;
        }
        
        if (chainLength > 0) {
            totalNonEmptyBuckets++;
        }
    }
    
    avgChainLength /= capacity;
    
    cout << "\n=== Estadísticas Detalladas ===" << endl;
    cout << "Buckets vacíos: " << emptyBuckets << " (" << (emptyBuckets * 100.0 / capacity) << "%)" << endl;
    cout << "Buckets con 1 elemento: " << singleItemBuckets << " (" << (singleItemBuckets * 100.0 / capacity) << "%)" << endl;
    cout << "Buckets con cadenas largas (>5): " << longChains << " (" << (longChains * 100.0 / capacity) << "%)" << endl;
    cout << "Longitud promedio de cadena: " << avgChainLength << endl;
    if (totalNonEmptyBuckets > 0) {
        cout << "Longitud promedio de cadena no vacía: " << (double)size / totalNonEmptyBuckets << endl;
    }
    cout << "Eficiencia de la función hash: " << (singleItemBuckets * 100.0 / size) << "% de elementos en buckets únicos" << endl;
}