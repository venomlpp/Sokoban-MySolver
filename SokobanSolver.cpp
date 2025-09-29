// SokobanSolver.cpp (versión corregida: añade isSimpleDeadlock / isCornerDeadlock,
// corrige la tabla de direcciones y permite desactivar la poda de deadlocks)

#include "SokobanSolver.h"
#include <climits>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define INT_INF (INT_MAX/4)

// Si quieres probar *sin* poda de deadlocks, pon esto a false:
const bool USE_DEADLOCK_PRUNING = true;

// helpers (no STL)
static int my_abs(int v) { return v < 0 ? -v : v; }
static int popcount_uint(unsigned int v) {
    int c = 0; while (v) { c += (v & 1); v >>= 1; } return c;
}

// min matching DP boxes->goals (admisible). Usa arrays y malloc—sin STL.
// Si goals > 20 hace greedy fallback.
static int minMatchingDP(const State* s, const Board* board) {
    int n = s->numBoxes;
    int m = board->numGoals;

    // listar cajas que NO están ya en goal
    int *bx = (int*) malloc(n * sizeof(int));
    int *by = (int*) malloc(n * sizeof(int));
    int nb = 0;
    for (int i=0;i<n;i++) {
        bool onGoal = false;
        for (int j=0;j<m;j++) if (s->boxX[i]==board->goalX[j] && s->boxY[i]==board->goalY[j]) { onGoal=true; break; }
        if (!onGoal) { bx[nb] = s->boxX[i]; by[nb] = s->boxY[i]; nb++; }
    }
    if (nb == 0) { free(bx); free(by); return 0; }
    if (m < nb) { free(bx); free(by); return INT_INF; }

    if (m > 20) {
        // greedy fallback
        char *used = (char*) malloc(m);
        for (int j=0;j<m;j++) used[j]=0;
        int total=0;
        for (int i=0;i<nb;i++) {
            int best=INT_INF, bi=-1;
            for (int j=0;j<m;j++) if (!used[j]) {
                int d = my_abs(bx[i]-board->goalX[j]) + my_abs(by[i]-board->goalY[j]);
                if (d < best) { best = d; bi=j; }
            }
            if (bi>=0) { used[bi]=1; total += best; }
        }
        free(used); free(bx); free(by);
        return total;
    }

    int FULL = 1 << m;
    int *dp = (int*) malloc(FULL * sizeof(int));
    for (int i=0;i<FULL;i++) dp[i] = INT_INF;
    dp[0]=0;

    for (int mask=0; mask < FULL; ++mask) {
        int k = popcount_uint((unsigned)mask);
        if (k >= nb) continue;
        int bxk = bx[k], byk = by[k];
        for (int j=0;j<m;j++) {
            if (mask & (1<<j)) continue;
            int cost = my_abs(bxk - board->goalX[j]) + my_abs(byk - board->goalY[j]);
            int nmask = mask | (1<<j);
            if (dp[nmask] > dp[mask] + cost) dp[nmask] = dp[mask] + cost;
        }
    }

    int best = INT_INF;
    for (int mask=0; mask < FULL; ++mask) {
        if (popcount_uint((unsigned)mask) == (unsigned)nb) {
            if (dp[mask] < best) best = dp[mask];
        }
    }

    free(dp);
    free(bx);
    free(by);
    return best;
}

// comparar solo configuracion de cajas (asume canonicalize)
static bool sameBoxes(const State* a, const State* b) {
    if (a->numBoxes != b->numBoxes) return false;
    for (int i=0;i<a->numBoxes;i++) if (a->boxX[i] != b->boxX[i] || a->boxY[i] != b->boxY[i]) return false;
    return true;
}

/* ----------------- DEADLOCK SIMPLE (implementación) -----------------
   Basado en tu versión previa: detecta corner deadlocks y bloqueos por paredes opuestas.
   Esta función asume las coordenadas (boxX, boxY) son la posición de la caja
   *después* del empuje (el destino).
*/
bool SokobanSolver::isCornerDeadlock(int x, int y) {
    bool wallLeft = board->isWall(x, y-1) || !board->isValidPosition(x, y-1);
    bool wallRight = board->isWall(x, y+1) || !board->isValidPosition(x, y+1);
    bool wallUp = board->isWall(x-1, y) || !board->isValidPosition(x-1, y);
    bool wallDown = board->isWall(x+1, y) || !board->isValidPosition(x+1, y);

    return (wallLeft && wallUp) || (wallLeft && wallDown) || (wallRight && wallUp) || (wallRight && wallDown);
}

bool SokobanSolver::isSimpleDeadlock(State* state, int boxX, int boxY) {
    // Si está en goal -> no es deadlock
    for (int g = 0; g < board->numGoals; ++g) {
        if (boxX == board->goalX[g] && boxY == board->goalY[g]) return false;
    }

    // esquina simple
    if (isCornerDeadlock(boxX, boxY)) return true;

    // bloqueo por paredes opuestas o cajas adyacentes en direcciones opuestas
    // Contamos paredes y cajas adyacentes
    int walls = 0, adjacentBoxes = 0;
    int dxs[4] = {-1,0,1,0};
    int dys[4] = {0,1,0,-1};

    for (int d=0; d<4; ++d) {
        int nx = boxX + dxs[d];
        int ny = boxY + dys[d];
        if (!board->isValidPosition(nx, ny) || board->isWall(nx, ny)) {
            walls++;
            continue;
        }
        // verificar si hay caja en nx,ny
        for (int i=0;i<state->numBoxes;i++) {
            if (state->boxX[i] == nx && state->boxY[i] == ny) { adjacentBoxes++; break; }
        }
    }

    if (walls >= 2) {
        // si las paredes están en direcciones opuestas (horizontal o vertical), consideramos deadlock
        bool horizontalBlock = (board->isWall(boxX, boxY-1) || !board->isValidPosition(boxX, boxY-1)) &&
                              (board->isWall(boxX, boxY+1) || !board->isValidPosition(boxX, boxY+1));
        bool verticalBlock = (board->isWall(boxX-1, boxY) || !board->isValidPosition(boxX-1, boxY)) &&
                             (board->isWall(boxX+1, boxY) || !board->isValidPosition(boxX+1, boxY));
        if (horizontalBlock || verticalBlock) return true;
    }

    return false;
}
/* ------------------------------------------------------------------ */

// Constructor / destructor
SokobanSolver::SokobanSolver(State* initialState, Board* board) {
    this->board = board;
    openList = new Heap(1000);
    closedList = new HashTable(10000, 0.75);

    initialState->costo = 0;
    int matching = minMatchingDP(initialState, board);
    if (matching >= INT_INF/2) initialState->heuristic = INT_INF;
    else initialState->heuristic = matching * board->pushCost;
    initialState->f_cost = initialState->costo + initialState->heuristic;

    openList->push(initialState);
}

SokobanSolver::~SokobanSolver() {
    delete openList;
    delete closedList;
}

bool SokobanSolver::isGoalState(State* state) {
    return state->boxesLeft == 0;
}

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

bool SokobanSolver::solve() {
    int statesExplored = 0;

    // tabla de direcciones consistente con tu Operation array:
    // operations was: Operation(0, -1), Operation(0, 1), Operation(-1, 0), Operation(1, 0)
    // interpretacion: dx = cambio fila, dy = cambio columna
    int dxLocal[4] = {0, 0, -1, 1};
    int dyLocal[4] = {-1, 1, 0, 0};

    while (!openList->isEmpty() && statesExplored < 2000000) {
        statesExplored++;
        State* current = openList->pop();

        if (statesExplored % 5000 == 0) {
            cout << "Estados: " << statesExplored << ", Open: " << openList->size 
                 << ", g=" << current->costo << ", h=" << current->heuristic << endl;
        }

        if (isGoalState(current)) {
            cout << "SOLUCIÓN ENCONTRADA en " << statesExplored << " estados!" << endl;
            current->printPath();
            cout << " ✅ ¡Solución encontrada!" << endl;
            return true;
        }

        closedList->insert(current);

        // generar vecinos (4 direcciones)
        for (int i = 0; i < 4; ++i) {
            // verificar ejecución posible
            if (!operations[i].canExecute(current, board)) continue;

            // calcular nx,ny con la tabla correcta
            int nx = current->x + dxLocal[i];
            int ny = current->y + dyLocal[i];

            // si es push -> comprobar si el empuje llevaría a deadlock simple (sólo si activado)
            bool isPush = operations[i].isPush(current);
            if (isPush && USE_DEADLOCK_PRUNING) {
                // encontrar índice de la caja empujada
                int boxIdx = -1;
                for (int b=0;b<current->numBoxes;b++) {
                    if (current->boxX[b] == nx && current->boxY[b] == ny) { boxIdx = b; break; }
                }
                if (boxIdx >= 0) {
                    int destX = nx + dxLocal[i];
                    int destY = ny + dyLocal[i];
                    if (isSimpleDeadlock(current, destX, destY)) continue;
                }
            }

            State* newState = operations[i].execute(current);
            if (newState == nullptr) continue;

            if (!isValid(newState)) { delete newState; continue; }

            // poda trivial: evitar push-undo inmediato (si nuevo estado de cajas == abuelo)
            if (isPush && current->parent != nullptr) {
                if (sameBoxes(newState, current->parent)) { delete newState; continue; }
            }

            // closed check
            if (closedList->contains(newState)) { delete newState; continue; }

            // actualizar g y energia y boxesLeft
            if (isPush) {
                newState->costo = current->costo + board->pushCost;
                newState->energia = current->energia - board->pushCost;
                if (operations[i].isPushedToGoal(current, board)) newState->boxesLeft = current->boxesLeft - 1;
                if (operations[i].isPushedOutOfGoal(current, board)) newState->boxesLeft = current->boxesLeft + 1;
            } else {
                newState->costo = current->costo + board->moveCost;
                newState->energia = current->energia - board->moveCost;
                newState->boxesLeft = current->boxesLeft;
            }

            // heuristica: minMatchingDP * pushCost (admisible)
            int matching = minMatchingDP(newState, board);
            if (matching >= INT_INF/2) newState->heuristic = INT_INF;
            else newState->heuristic = matching * board->pushCost;

            newState->f_cost = newState->costo + newState->heuristic;

            openList->push(newState);
        } // for dirs
    } // while

    cout << "NO SE ENCONTRÓ SOLUCIÓN después de " << statesExplored << " estados" << endl;
    return false;
}