// tests/test_state.cpp
#include "../State.h"
#include <cassert>
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    cout << "RUNNING State basic tests...\n";

    // --- Test 1: constructor con llaves y lockedBoxesChar ---
    int bx3[] = {5, 2, 4}; // desordenadas a propósito
    int by3[] = {6, 3, 8};
    char locked[] = {'A', '\0', 'B'}; // caja 0 bloqueada 'A', caja 2 bloqueada 'B'
    int keyX[] = {7, 1};
    int keyY[] = {2, 9};
    char keyCh[] = {'a', 'b'};
    State s3(1, 1, bx3, by3, locked, 3, 3, keyX, keyY, keyCh, 2, 200);

    // copia por constructor de copia
    State s4 = s3;
    assert(s3.equals(&s4) && "s3 debe ser igual a s4 luego del copy-constructor");
    cout << "Test1 passed: constructor con llaves y copy-constructor OK\n";

    // --- Test 2: clone (deep copy) ---
    State* s5 = s3.clone();
    assert(s3.equals(s5) && "clone debe producir estado igual");

    // Modificamos la clon para comprobar deep copy
    s5->x = 99;
    s5->boxX[0] = 123;
    // el original no debe verse afectado
    assert(s3.x != s5->x && "modificar clone.x no debe cambiar s3.x");
    assert(s3.boxX[0] != s5->boxX[0] && "modificar clone.boxX no debe cambiar s3.boxX");
    cout << "Test2 passed: clone es deep copy\n";

    // --- Test 3: equals considera currentKey y numKeys/locked ---
    State s6 = s3;
    // cambiar currentKey en s6 lo hace distinto
    s6.currentKey = 'a'; // s3.currentKey inicial es 0
    assert(!s3.equals(&s6) && "diferente currentKey => equals debe ser false");
    cout << "Test3 passed: equals considera currentKey\n";

    // --- Test 4: canonicalize mantiene lockedBoxes emparejados a cajas ---
    int bx7[] = {10, 2};
    int by7[] = {10, 3};
    char locked7[] = {'X', 'Y'}; 
    State s7(0,0,bx7,by7,locked7,2,2,nullptr,nullptr,nullptr,0,50);
    // tras canonicalize la caja con menor x (2,3) debe estar en índice 0 y su lock correspondiente 'Y'
    assert(s7.boxX[0] == 2 && s7.boxY[0] == 3);
    assert(s7.boxX[1] == 10 && s7.boxY[1] == 10);
    assert(s7.lockedBoxesChar[0] == 'Y' && s7.lockedBoxesChar[1] == 'X');
    cout << "Test4 passed: canonicalize empareja lockedBoxesChar con cajas\n";

    // Print states (visual check; no requerido por asserts)
    cout << "\nSample state (s3):\n";
    s3.printState();

    cout << "\nAll tests passed!\n";
    delete s5;
    return 0;
}