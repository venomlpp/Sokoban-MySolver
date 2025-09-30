#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include "State.h"

using namespace std;

class Heap {
    public:
    State **heap; // Arreglo de puntero a State
    int maxsize;
    int size;
    
    Heap();
    Heap(int maxsize);
    ~Heap();

    State * pop(); // saca la raiz y si esta vacio retorna nulo
    void push(State *s); // agrega un elemento a la pila y autoincrementa si es necesario
    bool isEmpty();

    void bubbleDown(int index); // sirve para arreglar el arbol y mantener como menor al padre desde indice i
    void bubbleUp(int index); // sirve para arreglar el arbol y mantener como menor al hijo desde indice i
    void swap(int i, int j); // intercambia los elemento i y j del arreglo

    int left(int index);
    int right(int index);
    int parent(int index);
};

#endif