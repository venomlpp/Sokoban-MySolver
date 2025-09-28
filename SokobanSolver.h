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
    //void printSolution() const;
    //^aún no decido como printear

    bool isValid(State* state) const; //si el movimiento es válido (no pasa por murallas, no pasa por cajas, etc)
    
private:
    Board* board;
    Heap* openList; // Estados por explorar
    HashTable* closedList; // Estados ya explorados
    Operation operations[4] = {
        Operation(0, -1), // Up
        Operation(0, 1),  // Down
        Operation(-1, 0), // Left
        Operation(1, 0)   // Right
    };

    // Métodos auxiliares
    bool isGoalState(State* state);
    int getHeuristic(State* state);
    int getAdvancedHeuristic(State* state); //heurística más avanzada
    int getEvenBetterHeuristic(State* state); //heurística aún más avanzada
    int getOptimizedMultiBoxHeuristic(State* state); //optimizada para +1 caja
    int getActuallyEvenBetterHeuristic(State* state); //heurística aún más avanzada
    bool isBoxPushable(State* state, int boxIndex) const;
    bool boxOnGoal(State* state, int boxIndex) const;
    int getDirectionAwareHeuristic(State* state); //heurística que considera dirección del jugador
    int getAggressiveDirectionHeuristic(State* state); //heurística agresiva que considera dirección del jugador
    int getFacil6NuclearHeuristic(State* state); //heurística fácil para niveles con 6 cajas
};

#endif //SOKOBANSOLVER_H