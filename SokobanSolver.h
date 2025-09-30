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
};

#endif //SOKOBANSOLVER_H
