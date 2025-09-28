#include "SokobanSolver.h"
#define INT_MAX 2147483647

SokobanSolver::SokobanSolver(State* initialState, Board* board) {
    this->board = board;
    openList = new Heap(1000);
    closedList = new HashTable(10000, 0.75);

    // Inicializar estado (asegurarse canonicalize ya hecho por constructor)
    initialState->costo = 0;
    initialState->heuristic = getHeuristic(initialState);
    initialState->f_cost = initialState->costo + initialState->heuristic;

    openList->push(initialState);
}

SokobanSolver::~SokobanSolver() {
    delete openList;
    delete closedList;
}

bool SokobanSolver::solve() {
    int statesExplored = 0;

    while (!openList->isEmpty() && statesExplored < 2000000) {
        statesExplored++;
        State *currentState = openList->pop();

        if (statesExplored % 5000 == 0) {
            cout << "Estados: " << statesExplored 
                 << ", Open: " << openList->size
                 << ", Boxes left: " << currentState->boxesLeft 
                 << ", Heurística: " << currentState->heuristic << endl;
        }

        // Si es objetivo, print path
        if (isGoalState(currentState)) {
            cout << "SOLUCIÓN ENCONTRADA en " << statesExplored << " estados!" << endl;
            currentState->printPath();
            cout << "✅ ¡Solución encontrada!" << endl;
            return true;
        }

        // insertar en closed
        closedList->insert(currentState);

        // generar sucesores
        for (int i = 0; i < 4; i++) {
            // Primero asegurarse que la operación es ejecutable (evita pushes a paredes/otras cajas)
            if (!operations[i].canExecute(currentState, board)) continue;

            State *newState = operations[i].execute(currentState); //"neighbor"

            if (newState == nullptr) continue;
            if (!isValid(newState)) { delete newState; continue; }

            if (!closedList->contains(newState)) {
                // calcular costo y energía
                if (operations[i].isPush(currentState)) {
                    newState->costo = currentState->costo + board->pushCost;
                    newState->energia = currentState->energia - board->pushCost;
                    if (operations[i].isPushedToGoal(currentState, board)) {
                        newState->boxesLeft = currentState->boxesLeft - 1;
                    }
                    if (operations[i].isPushedOutOfGoal(currentState, board)) {
                        newState->boxesLeft = currentState->boxesLeft + 1;
                    }
                } else {
                    newState->costo = currentState->costo + board->moveCost;
                    newState->energia = currentState->energia - board->moveCost;
                    newState->boxesLeft = currentState->boxesLeft;
                }

                // heurística robusta: greedy matching + distancia jugador->caja más cercana
                newState->heuristic = getHeuristic(newState);
                newState->f_cost = newState->costo + newState->heuristic;

                openList->push(newState);
            } else {
                delete newState;
            }
        }
    }

    cout << "NO SE ENCONTRÓ SOLUCIÓN después de " << /*statesExplored*/ "muchos" << " estados" << endl;
    return false;
}

bool SokobanSolver::isSolved() const {
    return openList->isEmpty();
}

bool SokobanSolver::isValid(State* state) const {
    int x = state->x;
    int y = state->y;
    // verificacion para jugador
    if (!board->isValidPosition(x, y)) return false;
    if (board->isWall(x, y)) return false;

    // verificacion para cajas
    for (int i = 0; i < state->numBoxes; i++) {
        if (!board->isValidPosition(state->boxX[i], state->boxY[i])) return false;
        if (board->isWall(state->boxX[i], state->boxY[i])) return false;
        for (int j = i + 1; j < state->numBoxes; j++) {
            if (state->boxX[i] == state->boxX[j] && state->boxY[i] == state->boxY[j]) {
                return false; // dos cajas en la misma posición
            }
        }
    }

    // verificacion de energia
    if (state->energia < 0) return false;
    return true;
}

bool SokobanSolver::isGoalState(State* state) {
    return state->boxesLeft == 0;
}

// ---------------- Heurística: Greedy matching + distancia jugador->caja más cercana ----------------
int SokobanSolver::getHeuristic(State* state) {
    if (state->boxesLeft == 0) return 0;

    // Greedy matching: para cada caja asignar el goal más cercano no usado
    int totalDistance = 0;
    int gcount = board->numGoals;
    int bcount = state->numBoxes;
    bool usedGoal[64] = {false}; // asumir un tope razonable; si necesitas más, aumentar
    for (int i = 0; i < bcount; i++) {
        int bx = state->boxX[i], by = state->boxY[i];
        // si caja ya está en goal, no sumar
        bool onGoal = false;
        for (int j = 0; j < gcount; j++) {
            if (bx == board->goalX[j] && by == board->goalY[j]) {
                onGoal = true;
                break;
            }
        }
        if (onGoal) continue;

        int minDist = INT_MAX;
        int bestGoal = -1;
        for (int j = 0; j < gcount; j++) {
            if (usedGoal[j]) continue;
            int dist = abs(bx - board->goalX[j]) + abs(by - board->goalY[j]);
            if (dist < minDist) {
                minDist = dist;
                bestGoal = j;
            }
        }
        if (bestGoal != -1) {
            usedGoal[bestGoal] = true;
            totalDistance += minDist;
        } else {
            // si no hay goal libre (caso raro), usar min overall
            int fallback = INT_MAX;
            for (int j = 0; j < gcount; j++) {
                int dist = abs(bx - board->goalX[j]) + abs(by - board->goalY[j]);
                if (dist < fallback) fallback = dist;
            }
            if (fallback < INT_MAX) totalDistance += fallback;
        }
    }

    // Distancia jugador -> caja más cercana (entre cajas que NO estén en goal)
    int minPlayerToBox = INT_MAX;
    for (int i = 0; i < bcount; i++) {
        int bx = state->boxX[i], by = state->boxY[i];
        bool onGoal = false;
        for (int j = 0; j < gcount; j++) if (bx == board->goalX[j] && by == board->goalY[j]) { onGoal = true; break; }
        if (onGoal) continue;
        int pd = abs(state->x - bx) + abs(state->y - by);
        if (pd < minPlayerToBox) minPlayerToBox = pd;
    }
    if (minPlayerToBox == INT_MAX) minPlayerToBox = 0;

    // combinación: matching total + (playerToBox) con peso menor
    return totalDistance + (minPlayerToBox);
}

// ---------------- Deadlock detection corregida ----------------
bool SokobanSolver::isSimpleDeadlock(State* state, int boxX, int boxY) {
    // Si la caja está en goal, no es deadlock
    for (int j = 0; j < board->numGoals; j++) {
        if (boxX == board->goalX[j] && boxY == board->goalY[j]) return false;
    }

    // Deadlock en esquina (dos paredes adyacentes)
    if (isCornerDeadlock(boxX, boxY)) return true;

    // Deadlock por paredes en direcciones opuestas o cajas bloqueando
    int walls = 0;
    int adjacentBoxes = 0;

    // vecinos: (fila,col) -> up=(x-1,y), right=(x,y+1), down=(x+1,y), left=(x,y-1)
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, 1, 0, -1};

    for (int d = 0; d < 4; d++) {
        int nx = boxX + dx[d];
        int ny = boxY + dy[d];
        if (!board->isValidPosition(nx, ny) || board->isWall(nx, ny)) {
            walls++;
        } else {
            // mirar si hay caja
            for (int i = 0; i < state->numBoxes; i++) {
                if (state->boxX[i] == nx && state->boxY[i] == ny) { adjacentBoxes++; break; }
            }
        }
    }

    // Si hay 2 paredes en direcciones opuestas horizontally or vertically -> deadlock
    bool horizontalBlock = ( (board->isWall(boxX, boxY-1) || !board->isValidPosition(boxX, boxY-1)) &&
                             (board->isWall(boxX, boxY+1) || !board->isValidPosition(boxX, boxY+1)) );
    bool verticalBlock = ( (board->isWall(boxX-1, boxY) || !board->isValidPosition(boxX-1, boxY)) &&
                           (board->isWall(boxX+1, boxY) || !board->isValidPosition(boxX+1, boxY)) );
    if (horizontalBlock || verticalBlock) return true;

    return false;
}

bool SokobanSolver::isCornerDeadlock(int x, int y) {
    // x = fila, y = col
    bool wallLeft  = board->isWall(x, y-1) || !board->isValidPosition(x, y-1);
    bool wallRight = board->isWall(x, y+1) || !board->isValidPosition(x, y+1);
    bool wallUp    = board->isWall(x-1, y) || !board->isValidPosition(x-1, y);
    bool wallDown  = board->isWall(x+1, y) || !board->isValidPosition(x+1, y);

    return (wallLeft && wallUp) || (wallLeft && wallDown) || (wallRight && wallUp) || (wallRight && wallDown);
}