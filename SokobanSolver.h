#ifndef SOKOBANSOLVER_H
#define SOKOBANSOLVER_H

#include <iostream>
#include "State.h"
#include "Heap.h"
#include "HashTable.h"
#include "Board.h"
#include "Operation.h"

using namespace std;

class SokobanSolver {
public:
    SokobanSolver(State* initialState, Board* board);
    ~SokobanSolver();

    bool solve();
    bool isSolved() const;
    bool isValid(State* state) const;
    
private:
    Board* board;
    Heap* openList; // Estados por explorar
    HashTable* closedList; // Estados ya explorados

    // OPERACIONES: dx = cambio fila, dy = cambio columna
    Operation operations[4] = {
        Operation(-1, 0), // Up
        Operation( 1, 0), // Down
        Operation( 0,-1), // Left
        Operation( 0, 1)  // Right
    };

    // Métodos auxiliares
    bool isGoalState(State* state);
    int getHeuristic(State* state); // ahora implementa Greedy matching + distancia a caja más cercana

    // Detección de deadlocks simple
    bool isSimpleDeadlock(State* state, int boxX, int boxY);
    bool isCornerDeadlock(int x, int y);

    // Utilidades
    int abs(int x) { return (x < 0) ? -x : x; }
    int min(int a, int b) { return (a < b) ? a : b; }
    int max(int a, int b) { return (a > b) ? a : b; }
};

#endif //SOKOBANSOLVER_H
