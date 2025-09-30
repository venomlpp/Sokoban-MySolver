#include "SokobanSolver.h"
#include <climits>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <ctime> // para poder ver tiempo en ejecución

// comparar solo configuracion de cajas (asume canonicalizacion)
static bool sameBoxes(const State* a, const State* b) {
    if (a->numBoxes != b->numBoxes) return false;
    for (int i=0;i<a->numBoxes;i++) if (a->boxX[i] != b->boxX[i] || a->boxY[i] != b->boxY[i]) return false;
    return true;
}

// constructor
SokobanSolver::SokobanSolver(State* initialState, Board* board) {
    this->board = board;
    openList = new Heap(1000);
    closedList = new HashTable(1000, 0.75);
    initialState->costo = 0;
    initialState->heuristic = getHeuristic(initialState);
    initialState->f_cost = initialState->costo + initialState->heuristic;
    openList->push(initialState);
}

// destructor
SokobanSolver::~SokobanSolver() {
    delete openList;
    delete closedList;
}

// condicion final
bool SokobanSolver::isGoalState(State* state) {
    return state->boxesLeft == 0;
}

// verificacion validez estado
bool SokobanSolver::isValid(State* state) const {
    if (!board->isValidPosition(state->x, state->y) || board->isWall(state->x, state->y)) return false;
    for (int i=0;i<state->numBoxes;i++) {
        if (!board->isValidPosition(state->boxX[i], state->boxY[i])) return false;
        if (board->isWall(state->boxX[i], state->boxY[i])) return false;
        for (int j=i+1;j<state->numBoxes;j++) {
            if (state->boxX[i] == state->boxX[j] && state->boxY[i] == state->boxY[j]) return false;
        }
    }
    if (state->energia < 0) return false;
    return true;
}

// heuristica: suma de (distancia caja-objetivo más cercano) + (distancia jugador-caja más cercana) + (distancia jugador-llave necesaria más cercana)
int SokobanSolver::getHeuristic(State* state) {
    if (state->boxesLeft == 0) return 0;

    int nBoxes = state->numBoxes;
    int nGoals = board->numGoals;

    // Marcar objetivos usados por el greedy matching
    char *usedGoal = new char[nGoals];
    for (int j = 0; j < nGoals; ++j) usedGoal[j] = 0;

    int greedySum = 0;

    // Por cada caja que NO este ya en un goal, asignarle su goal libre más cercano
    for (int i = 0; i < nBoxes; ++i) {
        bool onGoal = false;
        for (int g = 0; g < nGoals; ++g) {
            if (state->boxX[i] == board->goalX[g] && state->boxY[i] == board->goalY[g]) {
                onGoal = true;
                break;
            }
        }
        if (onGoal) continue;

        int bestDist = INT_MAX;
        int bestG = -1;
        for (int g = 0; g < nGoals; ++g) {
            if (usedGoal[g]) continue;
            int d = abs(state->boxX[i] - board->goalX[g]) + abs(state->boxY[i] - board->goalY[g]);
            if (d < bestDist) {
                bestDist = d;
                bestG = g;
            }
        }
        if (bestG != -1) {
            usedGoal[bestG] = 1;
            greedySum += bestDist;
        } else if (bestDist != INT_MAX) {
            greedySum += bestDist;
        }
    }

    // Distancia mínima del jugador a una caja que aún no está en goal
    int minPlayerToBox = INT_MAX;
    for (int i = 0; i < nBoxes; ++i) {
        bool onGoal = false;
        for (int g = 0; g < nGoals; ++g) {
            if (state->boxX[i] == board->goalX[g] && state->boxY[i] == board->goalY[g]) { onGoal = true; break; }
        }
        if (onGoal) continue;
        int pd = abs(state->x - state->boxX[i]) + abs(state->y - state->boxY[i]);
        if (pd < minPlayerToBox) minPlayerToBox = pd;
    }
    if (minPlayerToBox == INT_MAX) minPlayerToBox = 0;

    // nuevo para llaves
    int minPlayerToNeededKey = INT_MAX;
    // Para cada caja bloqueada que no esté en goal
    for (int i = 0; i < nBoxes; ++i) {
        if (state->lockedBoxesChar[i] == '\0') continue;
        // si la caja ya está en goal, ignorar
        bool onGoal = false;
        for (int g = 0; g < nGoals; ++g) {
            if (state->boxX[i] == board->goalX[g] && state->boxY[i] == board->goalY[g]) { onGoal = true; break; }
        }
        if (onGoal) continue;
        // si jugador ya lleva la llave correcta, no hay distancia extra
        char neededKey = (char)(state->lockedBoxesChar[i] - 'A' + 'a');
        if (state->currentKey == neededKey) { minPlayerToNeededKey = 0; break; }
        // si llave está en suelo del estado, calcular distancia jugador->llave y actualizar mínimo
        for (int k = 0; k < state->numKeys; ++k) {
            if (state->keyChar[k] == neededKey) {
                int d = abs(state->x - state->keyX[k]) + abs(state->y - state->keyY[k]);
                if (d < minPlayerToNeededKey) minPlayerToNeededKey = d;
            }
        }
    }
    if (minPlayerToNeededKey == INT_MAX) minPlayerToNeededKey = 0;

    delete[] usedGoal;

    int h_pushes = greedySum * board->pushCost;
    int h_player = minPlayerToBox * board->moveCost;
    int h_key = minPlayerToNeededKey * board->moveCost;
    return h_pushes + h_player + h_key;
}

// solver del sokoban, A* search con poda push-undo y heuristica
bool SokobanSolver::solve() {
    int statesExplored = 0;
    clock_t start = clock();

    //tabla de direcciones para verificar movimientos antes de crear estados innecesarios
    //esta tabla coincide en cada i con solver.operations
    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = {  0, 0,-1, 1 };

    while (!openList->isEmpty()) {
        State* current = openList->pop();
        statesExplored++;

        if (isGoalState(current)) {
            clock_t end = clock(); // detener cronómetro
            double elapsed = double(end - start) / CLOCKS_PER_SEC;
            cout << "SOLUCIÓN ENCONTRADA en " << statesExplored << " estados y " << elapsed << " segundos" << endl;
            current->printPath();
            return true;
        }

        closedList->insert(current);
        // generar vecinos (4 direcciones)
        for (int i = 0; i < 4; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];
            //verificamos si el jugador puede moverse a newX,newY
            if (!board->isValidPosition(newX, newY) || board->isWall(newX, newY)) continue;
            //verificamos si empuja caja y si es válido
            bool isPush = false;
            int boxIdx = -1;
            for (int b = 0; b < current->numBoxes; b++) {
                if (current->boxX[b] == newX && current->boxY[b] == newY) {
                    isPush = true;
                    boxIdx = b;
                    break;
                }
            }
            if (isPush) {
                int newBoxX = newX + dx[i];
                int newBoxY = newY + dy[i];
                if (!board->isValidPosition(newBoxX, newBoxY) || board->isWall(newBoxX, newBoxY)) continue;
                //verificamos si hay otra caja en newBoxX,newBoxY
                bool full = false;
                for (int b = 0; b < current->numBoxes; b++) {
                    if (current->boxX[b] == newBoxX && current->boxY[b] == newBoxY) {
                        full = true; break;
                    }
                }
                if (full) continue;
            }
            if (!operations[i].canExecute(current, board)) continue;
            State *newState = operations[i].execute(current);
            if (newState == nullptr) continue;
            if (!isValid(newState)) { delete newState; continue; }
            //poda para evitar push-undo
            if (isPush && current->parent != nullptr) {
                //atención con la condición de las llaves
                if (sameBoxes(newState, current->parent)) {
                    delete newState;
                    continue;
                }
            }
            if (closedList->contains(newState)) { delete newState; continue; }
            //actualización variables costo, energia, boxesLeft
            if (isPush) {
                newState->costo = current->costo + board->pushCost;
                newState->energia = current->energia - board->pushCost;
                if (operations[i].isPushedToGoal(current, board)) newState->boxesLeft = current->boxesLeft - 1;
                if (operations[i].isPushedOutOfGoal(current, board)) newState->boxesLeft = current->boxesLeft + 1;
            } else {
                newState->costo = current->costo + board->moveCost;
                newState->energia = current->energia - board->moveCost;
            }
            newState->heuristic = getHeuristic(newState);
            newState->f_cost = newState->costo + newState->heuristic;
            openList->push(newState);
        }
    }
    // Si cae aquí, no hay solución
    clock_t end = clock(); // detener cronómetro
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "NO SE ENCONTRÓ SOLUCIÓN después de " << statesExplored << " estados y " << elapsed << " segundos" << endl;
    return false;
}