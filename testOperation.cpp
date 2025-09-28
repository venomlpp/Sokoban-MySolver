#include "Operation.h"
#include "State.h"
#include <iostream>
#include <cassert>

using namespace std;

void testConstructorAndMoveChar() {
    cout << "=== Test: Constructor y getMoveChar ===" << endl;
    
    Operation opUp(-1, 0);
    Operation opDown(1, 0);
    Operation opLeft(0, -1);
    Operation opRight(0, 1);
    Operation opInvalid(2, 2);
    
    assert(opUp.getMoveChar() == 'U');
    assert(opDown.getMoveChar() == 'D');
    assert(opLeft.getMoveChar() == 'L');
    assert(opRight.getMoveChar() == 'R');
    assert(opInvalid.getMoveChar() == '?');
    
    cout << "✓ Test de constructor y getMoveChar completado exitosamente\n" << endl;
}

void testExecuteBasicMovement() {
    cout << "=== Test: Execute - Movimiento básico ===" << endl;
    
    State* initialState = new State(2, 2, nullptr, nullptr, 0, 100);
    
    Operation opUp(-1, 0);
    State* newStateUp = opUp.execute(initialState);
    assert(newStateUp->x == 1);
    assert(newStateUp->y == 2);
    cout << "✓ Movimiento hacia arriba correcto" << endl;
    
    Operation opDown(1, 0);
    State* newStateDown = opDown.execute(initialState);
    assert(newStateDown->x == 3);
    assert(newStateDown->y == 2);
    cout << "✓ Movimiento hacia abajo correcto" << endl;
    
    Operation opLeft(0, -1);
    State* newStateLeft = opLeft.execute(initialState);
    assert(newStateLeft->x == 2);
    assert(newStateLeft->y == 1);
    cout << "✓ Movimiento hacia izquierda correcto" << endl;
    
    Operation opRight(0, 1);
    State* newStateRight = opRight.execute(initialState);
    assert(newStateRight->x == 2);
    assert(newStateRight->y == 3);
    cout << "✓ Movimiento hacia derecha correcto" << endl;
    
    delete initialState;
    delete newStateUp;
    delete newStateDown;
    delete newStateLeft;
    delete newStateRight;
    
    cout << "✓ Test de movimiento básico completado exitosamente\n" << endl;
}

void testExecuteWithBoxPush() {
    cout << "=== Test: Execute - Empuje de caja ===" << endl;
    
    int boxX[] = {3, 3};
    int boxY[] = {3, 3};
    State* initialState = new State(2, 3, boxX, boxY, 1, 100);
    
    Operation opDown(1, 0);
    State* newState = opDown.execute(initialState);
    
    assert(newState->x == 3);
    assert(newState->y == 3);
    assert(newState->boxX[0] == 4);
    assert(newState->boxY[0] == 3);
    
    cout << "✓ Empuje de caja correcto: Jugador(" << newState->x << "," << newState->y 
         << "), Caja(" << newState->boxX[0] << "," << newState->boxY[0] << ")" << endl;
    
    delete initialState;
    delete newState;
    cout << "✓ Test de empuje de caja completado exitosamente\n" << endl;
}

void testExecuteWithMultipleBoxes() {
    cout << "=== Test: Execute - Múltiples cajas ===" << endl;
    
    // CORREGIDO: Caja en el camino del movimiento
    int boxX[] = {1, 4, 6};    // Caja 0 en (1, 3) - EN EL CAMINO
    int boxY[] = {3, 4, 6};
    State* initialState = new State(1, 2, boxX, boxY, 3, 100);
    
    Operation opRight(0, 1);
    State* newState = opRight.execute(initialState);
    
    assert(newState->x == 1);
    assert(newState->y == 3);
    assert(newState->boxX[0] == 1);
    assert(newState->boxY[0] == 4);  // Caja empujada a (1, 4)
    assert(newState->boxX[1] == 4);
    assert(newState->boxY[1] == 4);
    assert(newState->boxX[2] == 6);
    assert(newState->boxY[2] == 6);
    
    cout << "✓ Movimiento con múltiples cajas correcto" << endl;
    
    delete initialState;
    delete newState;
    cout << "✓ Test de múltiples cajas completado exitosamente\n" << endl;
}

void testExecuteNoPush() {
    cout << "=== Test: Execute - Sin empuje ===" << endl;
    
    // Caja NO en el camino
    int boxX[] = {2, 2};
    int boxY[] = {2, 2};
    State* initialState = new State(1, 2, boxX, boxY, 1, 100);
    
    Operation opRight(0, 1);  // Movimiento a (1, 3) - NO hay caja ahí
    State* newState = opRight.execute(initialState);
    
    assert(newState->x == 1);
    assert(newState->y == 3);
    assert(newState->boxX[0] == 2);  // Caja NO se mueve
    assert(newState->boxY[0] == 2);
    
    cout << "✓ Movimiento sin empuje correcto" << endl;
    
    delete initialState;
    delete newState;
    cout << "✓ Test de no empuje completado exitosamente\n" << endl;
}

void testIsPush() {
    cout << "=== Test: isPush ===" << endl;
    
    int boxX[] = {3, 3};
    int boxY[] = {3, 3};
    State* stateWithBox = new State(2, 3, boxX, boxY, 1, 100);
    State* stateWithoutBox = new State(2, 3, nullptr, nullptr, 0, 100);
    
    Operation opDown(1, 0);
    Operation opUp(-1, 0);
    Operation opLeft(0, -1);
    Operation opRight(0, 1);
    
    assert(opDown.isPush(stateWithBox) == true);
    assert(opDown.isPush(stateWithoutBox) == false);
    assert(opUp.isPush(stateWithBox) == false);
    assert(opLeft.isPush(stateWithBox) == false);
    assert(opRight.isPush(stateWithBox) == false);
    
    cout << "✓ isPush funciona correctamente" << endl;
    
    delete stateWithBox;
    delete stateWithoutBox;
    cout << "✓ Test de isPush completado exitosamente\n" << endl;
}

void testEdgeCases() {
    cout << "=== Test: Casos límite ===" << endl;
    
    int boxX[] = {1, 1};
    int boxY[] = {1, 1};
    State* state = new State(0, 0, boxX, boxY, 1, 100);
    
    Operation zeroOp(0, 0);
    State* result = zeroOp.execute(state);
    
    assert(result->x == 0);
    assert(result->y == 0);
    assert(result->lastMove == '?');
    assert(zeroOp.isPush(state) == false);
    
    cout << "✓ Casos límite manejados correctamente" << endl;
    
    delete state;
    delete result;
    cout << "✓ Test de casos límite completado exitosamente\n" << endl;
}

int main() {
    cout << "=========================================" << endl;
    cout << "INICIANDO PRUEBAS DE LA CLASE OPERATION" << endl;
    cout << "=========================================\n" << endl;
    
    try {
        testConstructorAndMoveChar();
        testExecuteBasicMovement();
        testExecuteWithBoxPush();
        testExecuteWithMultipleBoxes();
        testExecuteNoPush();
        testIsPush();
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