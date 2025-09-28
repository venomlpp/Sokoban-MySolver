#include "Heap.h"
#include "State.h"
#include <iostream>
#include <cassert>

using namespace std;

void testConstructor() {
    cout << "=== Test: Constructores ===" << endl;
    
    // Test constructor por defecto
    Heap heap1;
    assert(heap1.size == 0);
    assert(heap1.maxsize == 100);
    assert(heap1.heap != nullptr);
    cout << "✓ Constructor por defecto funciona correctamente" << endl;
    
    // Test constructor con tamaño específico
    Heap heap2(50);
    assert(heap2.size == 0);
    assert(heap2.maxsize == 50);
    assert(heap2.heap != nullptr);
    cout << "✓ Constructor con tamaño específico funciona correctamente" << endl;
    
    cout << "✓ Test de constructores completado exitosamente\n" << endl;
}

void testPushAndBasicOperations() {
    cout << "=== Test: Operaciones básicas (push, isEmpty, size) ===" << endl;
    
    Heap heap(5); // Heap pequeño para probar redimensionamiento
    
    // Test isEmpty en heap vacío
    assert(heap.isEmpty() == true);
    assert(heap.size == 0);
    cout << "✓ Heap vacío identificado correctamente" << endl;
    
    // Crear estados de prueba simples
    State* state1 = new State(0, 0, nullptr, nullptr, 0, 100);
    state1->costo = 10;
    state1->heuristic = 5;
    
    State* state2 = new State(1, 1, nullptr, nullptr, 0, 100);
    state2->costo = 5;
    state2->heuristic = 3;
    
    State* state3 = new State(2, 2, nullptr, nullptr, 0, 100);
    state3->costo = 15;
    state3->heuristic = 2;
    
    // Test push
    heap.push(state1);
    assert(heap.isEmpty() == false);
    assert(heap.size == 1);
    cout << "✓ Primer push realizado correctamente (size = " << heap.size << ")" << endl;
    
    heap.push(state2);
    assert(heap.size == 2);
    cout << "✓ Segundo push realizado correctamente (size = " << heap.size << ")" << endl;
    
    heap.push(state3);
    assert(heap.size == 3);
    cout << "✓ Tercer push realizado correctamente (size = " << heap.size << ")" << endl;
    
    cout << "✓ Test de operaciones básicas completado exitosamente\n" << endl;
}

void testPopAndOrder() {
    cout << "=== Test: Pop y orden del heap ===" << endl;
    
    Heap heap;
    
    // Crear estados con diferentes costos totales (costo + heuristic)
    State* state1 = new State(0, 0, nullptr, nullptr, 0, 100);
    state1->costo = 20;    // Total: 20 + 5 = 25
    state1->heuristic = 5;
    
    State* state2 = new State(1, 1, nullptr, nullptr, 0, 100);
    state2->costo = 10;    // Total: 10 + 8 = 18 (MENOR - debería salir primero)
    state2->heuristic = 8;
    
    State* state3 = new State(2, 2, nullptr, nullptr, 0, 100);
    state3->costo = 15;    // Total: 15 + 10 = 25
    state3->heuristic = 10;
    
    State* state4 = new State(3, 3, nullptr, nullptr, 0, 100);
    state4->costo = 5;     // Total: 5 + 12 = 17 (MENOR de todos - debería salir primero)
    state4->heuristic = 12;
    
    // Insertar en orden arbitrario
    heap.push(state1);
    heap.push(state2);
    heap.push(state3);
    heap.push(state4);
    
    cout << "Estados insertados en orden arbitrario" << endl;
    cout << "State1: costo=20, heuristic=5, total=25" << endl;
    cout << "State2: costo=10, heuristic=8, total=18" << endl;
    cout << "State3: costo=15, heuristic=10, total=25" << endl;
    cout << "State4: costo=5, heuristic=12, total=17" << endl;
    
    // Test pop - debería sacar en orden de menor costo total
    State* popped;
    
    // Primer pop - debería ser state4 (total 17)
    popped = heap.pop();
    assert(popped == state4);
    assert(popped->costo == 5);
    assert(heap.size == 3);
    cout << "✓ Primer pop: State4 (total=17) - CORRECTO" << endl;
    
    // Segundo pop - debería ser state2 (total 18)
    popped = heap.pop();
    assert(popped == state2);
    assert(popped->costo == 10);
    assert(heap.size == 2);
    cout << "✓ Segundo pop: State2 (total=18) - CORRECTO" << endl;
    
    // Tercer pop - podría ser state1 o state3 (ambos total 25)
    popped = heap.pop();
    assert(popped->costo + popped->heuristic == 25);
    assert(heap.size == 1);
    cout << "✓ Tercer pop: Estado con total=25 - CORRECTO" << endl;
    
    // Cuarto pop - el que quedó
    popped = heap.pop();
    assert(popped->costo + popped->heuristic == 25);
    assert(heap.size == 0);
    cout << "✓ Cuarto pop: Último estado - CORRECTO" << endl;
    
    // Pop en heap vacío
    popped = heap.pop();
    assert(popped == nullptr);
    assert(heap.isEmpty() == true);
    cout << "✓ Pop en heap vacío retorna nullptr - CORRECTO" << endl;
    
    cout << "✓ Test de pop y orden completado exitosamente\n" << endl;
}

void testBubbleOperations() {
    cout << "=== Test: Operaciones bubbleUp y bubbleDown ===" << endl;
    
    Heap heap(10);
    
    // Crear estados específicos para probar el burbujeo
    State* states[5];
    for (int i = 0; i < 5; i++) {
        states[i] = new State(i, i, nullptr, nullptr, 0, 100);
    }
    
    // Asignar costos que fuerzan burbujeo
    states[0]->costo = 50; states[0]->heuristic = 0; // Total: 50
    states[1]->costo = 10; states[1]->heuristic = 0; // Total: 10 (debería subir)
    states[2]->costo = 30; states[2]->heuristic = 0; // Total: 30
    states[3]->costo = 5;  states[3]->heuristic = 0; // Total: 5 (debería ser raíz)
    states[4]->costo = 40; states[4]->heuristic = 0; // Total: 40
    
    // Insertar en orden que force bubbleUp
    cout << "Probando bubbleUp..." << endl;
    heap.push(states[0]); // Raíz: 50
    heap.push(states[1]); // 10 debería subir y convertirse en raíz
    assert(heap.heap[0] == states[1]); // 10 debería ser la nueva raíz
    cout << "✓ bubbleUp funciona: estado con costo 10 subió a raíz" << endl;
    
    heap.push(states[2]); // 30 se inserta
    heap.push(states[3]); // 5 debería subir y convertirse en raíz
    assert(heap.heap[0] == states[3]); // 5 debería ser la nueva raíz
    cout << "✓ bubbleUp funciona: estado con costo 5 subió a raíz" << endl;
    
    heap.push(states[4]); // 40 se inserta
    
    // Verificar el orden actual del heap
    cout << "Estado del heap después de inserciones:" << endl;
    cout << "Raíz: costo=" << heap.heap[0]->costo << endl;
    cout << "Hijo izquierdo: costo=" << heap.heap[1]->costo << endl;
    cout << "Hijo derecho: costo=" << heap.heap[2]->costo << endl;
    
    // Test bubbleDown con pop
    cout << "Probando bubbleDown con pop..." << endl;
    State* popped = heap.pop(); // Saca el 5
    assert(popped == states[3]);
    assert(popped->costo == 5);
    
    // Después de pop, bubbleDown debería reordenar
    // La nueva raíz debería ser el siguiente menor (10)
    assert(heap.heap[0] == states[1]);
    assert(heap.heap[0]->costo == 10);
    cout << "✓ bubbleDown funciona: nueva raíz es estado con costo 10" << endl;
    
    cout << "✓ Test de operaciones bubble completado exitosamente\n" << endl;
}

void testResize() {
    cout << "=== Test: Redimensionamiento automático ===" << endl;
    
    Heap heap(3); // Heap muy pequeño para forzar redimensionamiento
    
    cout << "Tamaño inicial: " << heap.maxsize << endl;
    
    // Insertar más elementos de la capacidad inicial
    for (int i = 0; i < 5; i++) {
        State* state = new State(i, i, nullptr, nullptr, 0, 100);
        state->costo = i * 10;
        state->heuristic = 0;
        heap.push(state);
        cout << "Insertado estado " << i << ", size=" << heap.size << ", maxsize=" << heap.maxsize << endl;
    }
    
    // Verificar que se redimensionó
    assert(heap.maxsize > 3);
    assert(heap.size == 5);
    cout << "✓ Redimensionamiento automático funcionó correctamente" << endl;
    
    // Verificar que todos los elementos están presentes y en orden
    int lastCost = -1;
    while (!heap.isEmpty()) {
        State* popped = heap.pop();
        assert(popped->costo >= lastCost); // Debería salir en orden ascendente
        lastCost = popped->costo;
    }
    cout << "✓ Todos los elementos recuperados en orden correcto después del redimensionamiento" << endl;
    
    cout << "✓ Test de redimensionamiento completado exitosamente\n" << endl;
}

void testEdgeCases() {
    cout << "=== Test: Casos límite ===" << endl;
    
    // Test con un solo elemento
    Heap heap1;
    State* singleState = new State(0, 0, nullptr, nullptr, 0, 100);
    singleState->costo = 100;
    heap1.push(singleState);
    
    assert(heap1.size == 1);
    assert(heap1.pop() == singleState);
    assert(heap1.isEmpty());
    cout << "✓ Heap con un solo elemento funciona correctamente" << endl;
    
    // Test con elementos duplicados
    Heap heap2;
    State* dup1 = new State(0, 0, nullptr, nullptr, 0, 100);
    State* dup2 = new State(1, 1, nullptr, nullptr, 0, 100);
    dup1->costo = dup2->costo = 10;
    dup1->heuristic = dup2->heuristic = 5;
    
    heap2.push(dup1);
    heap2.push(dup2);
    
    State* first = heap2.pop();
    State* second = heap2.pop();
    assert(first->costo == 10 && second->costo == 10);
    cout << "✓ Elementos con costos duplicados manejados correctamente" << endl;
    
    cout << "✓ Test de casos límite completado exitosamente\n" << endl;
}

int main() {
    cout << "=========================================" << endl;
    cout << "INICIANDO PRUEBAS DE LA CLASE HEAP" << endl;
    cout << "=========================================\n" << endl;
    
    try {
        testConstructor();
        testPushAndBasicOperations();
        testPopAndOrder();
        testBubbleOperations();
        testResize();
        testEdgeCases();
        
        cout << "=========================================" << endl;
        cout << "¡TODAS LAS PRUEBAS PASARON EXITOSAMENTE!" << endl;
        cout << "=========================================" << endl;
        
    } catch (const exception& e) {
        cerr << "❌ Error durante las pruebas: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}