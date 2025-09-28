#include "Heap.h"

Heap::Heap() {
//    cout << "Llamada al constructor sin argumentos" << endl;
    maxsize = 100; // tamaño inicial
    size = 0; // tamaño actual
    heap = new State*[maxsize]; // crea el arreglo de punteros a State
}

Heap::Heap(int maxsize) {
//    cout << "Llamada al constructor con argumentos" << endl;
    this->maxsize = maxsize; // tamaño inicial
    size = 0; // tamaño actual
    heap = new State*[maxsize]; // crea el arreglo de punteros a State
}

Heap::~Heap() {
    delete[] heap; // libera memoria del arreglo
}

// obtiene el menor, que es la raiz del Heap y reordena
State * Heap::pop() {
    if (size==0) {
        return nullptr; // es el elemnto NULL de C++
    }
    State * ret = heap[0];

    // mover el ultimo a la raiz
    heap[0] = heap[size-1];
    size--;

    bubbleDown(0); // burbujea hacia abajo

    return ret;
}

// agrega un nuevo elemento al Heap y lo posiciona correctamente
void Heap::push(State *s) {
    // aqui es donde se puede pasar de la cantidad de memoria disponible
    if (size==maxsize) {
        State ** newarr=new State*[2*maxsize]; 
        for(int i=0;i<size;i++) {
            newarr[i]=heap[i];
        }
        delete[] heap;
        heap=newarr;
        maxsize*=2;
    }

    heap[size]=s;
    size++;

    bubbleUp(size-1);
}

// aqui asumo que i y j estan con valores validos
void Heap::swap(int i, int j) {
    State * tmp=heap[i];
    heap[i]=heap[j];
    heap[j]=tmp;
}

int Heap::left(int index) {
    return 2 * index + 1;
}

int Heap::right(int index) {
    return 2 * index + 2;
}

// notar que se mejoro la formula, al ser int trunca automaticamente
int Heap::parent(int index) {
    return (index - 1) / 2;
}

// Se utiliza para el metodo push
void Heap::bubbleUp(int index) {
    // si no es la raiz entonces padre < hijo, sino se intercambia y prosigue recursivamente
    if (index==0) return; // ya es la raiz
    int p = parent(index);
    if (heap[p]->costo + heap[p]->heuristic > heap[index]->costo + heap[index]->heuristic) {
        swap(p, index);
        bubbleUp(p);
    }
}

// Se utiliza para el metodo pop
void Heap::bubbleDown(int index) {
    // buscamos el menor de los hijos y si no es mayor que su padre se intercambia
    int leftIndex = left(index);
    int rightIndex = right(index);
    int smallest = index; // quien debiese ser, pero se cambia al menor en caso contrario

    if (leftIndex < size && // se compara con el izq solo si existe
        heap[leftIndex]->costo + heap[leftIndex]->heuristic < heap[smallest]->costo + heap[smallest]->heuristic) {
        smallest = leftIndex;
    }
    if (rightIndex < size && // se compara con el der solo si existe
        heap[rightIndex]->costo + heap[rightIndex]->heuristic < heap[smallest]->costo + heap[smallest]->heuristic) {
        smallest = rightIndex;
    }
    if (smallest != index) { // si el menor no es el padre intercambiamos y seguimos recursivamente
        swap(index, smallest);
        bubbleDown(smallest);
    }
}

bool Heap::isEmpty() {
    return size == 0;
}