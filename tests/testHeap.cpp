#include "../Heap.h"
#include "../State.h"
#include <cassert>
#include <iostream>
using namespace std;

int main() {
    cout << "RUNNING Heap basic tests...\n";

    // --- Crear Heap con capacidad inicial ---
    Heap h(4);
    assert(h.isEmpty() && "El heap debe comenzar vacío");

    // --- Crear algunos estados dummy ---
    int bx[] = {1}, by[] = {1};
    char locked[] = {'\0'};
    State s1(0,0,bx,by,locked,1,1,nullptr,nullptr,nullptr,0,10); // costo+heuristic = 0
    State s2(0,0,bx,by,locked,1,1,nullptr,nullptr,nullptr,0,10); // le subimos heurística
    s2.heuristic = 5;
    State s3(0,0,bx,by,locked,1,1,nullptr,nullptr,nullptr,0,10); 
    s3.costo = 2; s3.heuristic = 1; // total = 3

    // --- Push en orden arbitrario ---
    h.push(&s2); // total = 5
    h.push(&s1); // total = 0
    h.push(&s3); // total = 3

    // --- El primero en salir debe ser s1 (total=0) ---
    State* first = h.pop();
    assert(first == &s1 && "El primer pop debe devolver el estado con costo+heuristic más bajo (s1)");
    cout << "First pop correct: costo+heuristic = " << first->costo + first->heuristic << "\n";

    // --- El siguiente debe ser s3 (total=3) ---
    State* second = h.pop();
    assert(second == &s3 && "El segundo pop debe devolver s3");
    cout << "Second pop correct: costo+heuristic = " << second->costo + second->heuristic << "\n";

    // --- Finalmente s2 (total=5) ---
    State* third = h.pop();
    assert(third == &s2 && "El tercer pop debe devolver s2");
    cout << "Third pop correct: costo+heuristic = " << third->costo + third->heuristic << "\n";

    // --- Y luego debe estar vacío ---
    assert(h.isEmpty() && "El heap debe quedar vacío tras 3 pops");

    cout << "All Heap tests passed!\n";
    return 0;
}
