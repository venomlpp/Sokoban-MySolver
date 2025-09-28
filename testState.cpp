#include "State.h"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

void testConstructorAndBasicProperties() {
    cout << "=== Test: Constructor y propiedades básicas ===" << endl;
    
    // Crear arrays de cajas de prueba
    int boxX[] = {1, 3, 5};
    int boxY[] = {2, 4, 6};
    int numBoxes = 3;
    int energia = 100;
    
    State state(2, 2, boxX, boxY, numBoxes, energia);
    
    // Verificar propiedades básicas
    assert(state.x == 2);
    assert(state.y == 2);
    assert(state.energia == 100);
    assert(state.numBoxes == 3);
    assert(state.boxesLeft == 3); // Inicialmente igual a numBoxes
    assert(state.costo == 0);
    assert(state.heuristic == 0);
    assert(state.parent == nullptr);
    assert(state.lastMove == ' ');
    
    cout << "✓ Propiedades básicas inicializadas correctamente" << endl;
    
    // Verificar que las cajas se copiaron correctamente
    assert(state.boxX != nullptr);
    assert(state.boxY != nullptr);
    assert(state.boxX[0] == 1); assert(state.boxY[0] == 2);
    assert(state.boxX[1] == 3); assert(state.boxY[1] == 4);
    assert(state.boxX[2] == 5); assert(state.boxY[2] == 6);
    
    cout << "✓ Arrays de cajas copiados correctamente" << endl;
    
    // Verificar que son copias independientes (no los mismos punteros)
    assert(state.boxX != boxX);
    assert(state.boxY != boxY);
    
    cout << "✓ Arrays son copias profundas (independientes)" << endl;
    
    cout << "✓ Test de constructor completado exitosamente\n" << endl;
}

void testCopyConstructor() {
    cout << "=== Test: Constructor de copia ===" << endl;
    
    // Crear estado original
    int boxX[] = {1, 2};
    int boxY[] = {1, 2};
    State original(0, 0, boxX, boxY, 2, 100);
    original.costo = 10;
    original.heuristic = 5;
    original.lastMove = 'R';
    
    // Crear copia
    State copia(original);
    
    // Verificar que los valores básicos se copiaron
    assert(copia.x == original.x);
    assert(copia.y == original.y);
    assert(copia.energia == original.energia);
    assert(copia.numBoxes == original.numBoxes);
    assert(copia.boxesLeft == original.boxesLeft);
    assert(copia.costo == original.costo);
    assert(copia.heuristic == original.heuristic);
    assert(copia.lastMove == original.lastMove);
    assert(copia.parent == original.parent);
    
    cout << "✓ Valores básicos copiados correctamente" << endl;
    
    // Verificar que los arrays de cajas son independientes
    assert(copia.boxX != original.boxX);
    assert(copia.boxY != original.boxY);
    assert(copia.boxX[0] == original.boxX[0]);
    assert(copia.boxY[0] == original.boxY[0]);
    assert(copia.boxX[1] == original.boxX[1]);
    assert(copia.boxY[1] == original.boxY[1]);
    
    cout << "✓ Arrays de cajas son copias profundas independientes" << endl;
    
    // Modificar la copia y verificar que el original no cambia
    copia.boxX[0] = 99;
    copia.boxY[0] = 99;
    assert(original.boxX[0] == 1); // Original no debe cambiar
    assert(original.boxY[0] == 1);
    
    cout << "✓ Modificaciones en la copia no afectan al original" << endl;
    
    cout << "✓ Test de constructor de copia completado exitosamente\n" << endl;
}

void testEqualsMethod() {
    cout << "=== Test: Método equals ===" << endl;
    
    // Estados idénticos
    int boxX1[] = {1, 2}; int boxY1[] = {1, 2};
    State state1(0, 0, boxX1, boxY1, 2, 100);
    
    int boxX2[] = {1, 2}; int boxY2[] = {1, 2};
    State state2(0, 0, boxX2, boxY2, 2, 100);
    
    // Estados diferentes en posición del jugador
    int boxX3[] = {1, 2}; int boxY3[] = {1, 2};
    State state3(1, 0, boxX3, boxY3, 2, 100);
    
    // Estados diferentes en energía
    int boxX4[] = {1, 2}; int boxY4[] = {1, 2};
    State state4(0, 0, boxX4, boxY4, 2, 90);
    
    // Estados diferentes en número de cajas
    int boxX5[] = {1, 2, 3}; int boxY5[] = {1, 2, 3};
    State state5(0, 0, boxX5, boxY5, 3, 100);
    
    // Estados diferentes en posición de cajas
    int boxX6[] = {1, 3}; int boxY6[] = {1, 3}; // Segunda caja diferente
    State state6(0, 0, boxX6, boxY6, 2, 100);
    
    // Test igualdad
    assert(state1.equals(&state2) == true);
    cout << "✓ Estados idénticos correctamente identificados como iguales" << endl;
    
    // Test diferencias
    assert(state1.equals(&state3) == false);
    cout << "✓ Estados con posición de jugador diferente correctamente distinguidos" << endl;
    
    assert(state1.equals(&state4) == false);
    cout << "✓ Estados con energía diferente correctamente distinguidos" << endl;
    
    assert(state1.equals(&state5) == false);
    cout << "✓ Estados con número de cajas diferente correctamente distinguidos" << endl;
    
    assert(state1.equals(&state6) == false);
    cout << "✓ Estados con posición de cajas diferente correctamente distinguidos" << endl;
    
    cout << "✓ Test de método equals completado exitosamente\n" << endl;
}

void testCloneMethod() {
    cout << "=== Test: Método clone ===" << endl;
    
    // Crear estado original con valores específicos
    int boxX[] = {1, 2, 3};
    int boxY[] = {4, 5, 6};
    State original(7, 8, boxX, boxY, 3, 100);
    original.costo = 15;
    original.heuristic = 10;
    original.lastMove = 'U';
    original.boxesLeft = 2; // Simular que una caja fue colocada
    
    // Crear clon
    State* clone = original.clone();
    
    // Verificar que los valores se copiaron correctamente
    assert(clone->x == original.x);
    assert(clone->y == original.y);
    assert(clone->energia == original.energia);
    assert(clone->numBoxes == original.numBoxes);
    assert(clone->boxesLeft == original.boxesLeft);
    assert(clone->costo == original.costo);
    assert(clone->heuristic == original.heuristic);
    assert(clone->lastMove == original.lastMove);
    assert(clone->parent == original.parent);
    
    cout << "✓ Valores básicos clonados correctamente" << endl;
    
    // Verificar que los arrays son independientes
    assert(clone->boxX != original.boxX);
    assert(clone->boxY != original.boxY);
    
    for (int i = 0; i < original.numBoxes; i++) {
        assert(clone->boxX[i] == original.boxX[i]);
        assert(clone->boxY[i] == original.boxY[i]);
    }
    
    cout << "✓ Arrays de cajas clonados como copias profundas" << endl;
    
    // Modificar el clon y verificar que el original no cambia
    clone->boxX[0] = 99;
    clone->boxY[0] = 99;
    assert(original.boxX[0] == 1); // Original no debe cambiar
    assert(original.boxY[0] == 4);
    
    cout << "✓ Modificaciones en el clon no afectan al original" << endl;
    
    // Limpiar memoria
    delete clone;
    
    cout << "✓ Test de método clone completado exitosamente\n" << endl;
}

void testPrintMethods() {
    cout << "=== Test: Métodos de impresión ===" << endl;
    
    // Crear estado de prueba
    int boxX[] = {1, 3};
    int boxY[] = {2, 4};
    State state(0, 0, boxX, boxY, 2, 100);
    state.costo = 25;
    state.heuristic = 15;
    state.lastMove = 'R';
    state.boxesLeft = 1; // Una caja colocada
    
    cout << "Probando printState():" << endl;
    cout << "---------------------" << endl;
    state.printState();
    cout << endl;
    
    cout << "✓ printState() ejecutado sin errores" << endl;
    
    // Test de printPath con un camino simple
    State* state2 = new State(1, 1, boxX, boxY, 2, 95);
    state2->lastMove = 'D';
    state2->parent = &state;
    
    State* state3 = new State(2, 2, boxX, boxY, 2, 90);
    state3->lastMove = 'L';
    state3->parent = state2;
    
    cout << "Probando printPath():" << endl;
    cout << "--------------------" << endl;
    cout << "Camino esperado: R D L" << endl;
    cout << "Camino actual: ";
    state3->printPath();
    cout << endl;
    
    cout << "✓ printPath() ejecutado sin errores" << endl;
    
    // Limpiar memoria
    delete state2;
    delete state3;
    
    cout << "✓ Test de métodos de impresión completado exitosamente\n" << endl;
}

void testEdgeCases() {
    cout << "=== Test: Casos límite ===" << endl;
    
    // Test con 1 caja
    int singleBoxX[] = {5};
    int singleBoxY[] = {5};
    State state2(0, 0, singleBoxX, singleBoxY, 1, 100);
    assert(state2.numBoxes == 1);
    assert(state2.boxesLeft == 1);
    assert(state2.boxX[0] == 5);
    assert(state2.boxY[0] == 5);
    cout << "✓ Estado con una caja manejado correctamente" << endl;
    cout << "✓ Test de casos límite completado exitosamente\n" << endl;
}

void testParentRelationship() {
    cout << "=== Test: Relación parent (árbol de estados) ===" << endl;
    
    int boxX[] = {1, 2};
    int boxY[] = {1, 2};
    
    // Crear una secuencia de estados padre-hijo
    State* root = new State(0, 0, boxX, boxY, 2, 100);
    root->lastMove = ' ';
    
    State* child1 = new State(1, 0, boxX, boxY, 2, 95);
    child1->lastMove = 'D';
    child1->parent = root;
    
    State* child2 = new State(1, 1, boxX, boxY, 2, 90);
    child2->lastMove = 'R';
    child2->parent = child1;
    
    // Verificar la relación parent
    assert(root->parent == nullptr);
    assert(child1->parent == root);
    assert(child2->parent == child1);
    
    cout << "✓ Relaciones parent establecidas correctamente" << endl;
    
    // Verificar el camino
    cout << "Camino desde child2: ";
    child2->printPath();
    cout << endl;
    
    // Limpiar memoria
    delete child2;
    delete child1;
    delete root;
    
    cout << "✓ Test de relación parent completado exitosamente\n" << endl;
}

int main() {
    cout << "=========================================" << endl;
    cout << "INICIANDO PRUEBAS DE LA CLASE STATE" << endl;
    cout << "=========================================\n" << endl;
    
    try {
        testConstructorAndBasicProperties();
        testCopyConstructor();
        testEqualsMethod();
        testCloneMethod();
        testPrintMethods();
        testEdgeCases();
        testParentRelationship();
        
        cout << "=========================================" << endl;
        cout << "¡TODAS LAS PRUEBAS PASARON EXITOSAMENTE!" << endl;
        cout << "=========================================" << endl;
        
    } catch (const exception& e) {
        cerr << "❌ Error durante las pruebas: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}