//Generamos un solver con el algoritmo A* con heurística
#include "SokobanSolver.h"
#define INT_MAX 2147483647

SokobanSolver::SokobanSolver(State* initialState, Board* board) {
    this->board = board;
    openList = new Heap(1000);
    closedList = new HashTable(1000, 0.75);
    
    // Calcular heurística inicial
    initialState->heuristic = getHeuristic(initialState);
    openList->push(initialState);
}

SokobanSolver::~SokobanSolver() {
    delete openList;
    delete closedList;
}

bool SokobanSolver::solve() {
    int statesExplored = 0;

    while (!openList->isEmpty() && statesExplored < 500000) {
        statesExplored++;
        State *currentState = openList->pop();

        if (statesExplored % 5000 == 0) {
            cout << "Estados: " << statesExplored 
                 << ", Open: " << openList->size
                 << ", Boxes left: " << currentState->boxesLeft 
                 << ", Heurística: " << currentState->heuristic << endl;
            }
        closedList->insert(currentState);
        if (isGoalState(currentState)) {
            cout << "SOLUCIÓN ENCONTRADA en " << statesExplored << " estados!" << endl;
            currentState->printPath();
            return true;
        }
        for (int i = 0; i < 4; i++) {
            //crear nuevos estados con cada operación
            State *newState = operations[i].execute(currentState); //"neighbor"
            if (isValid(newState) && !closedList->contains(newState)) {
                //calcular costo
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
                }
                //calcular heurística
                newState->heuristic = getFacil6NuclearHeuristic(newState);
                //después de calcular la heurística, lo añadimos a la lista abierta
                openList->push(newState);
            }
            else {
                delete newState; // liberar memoria si no es válido o ya explorado
            }
        }
    }
    return false; // No se encontró solución
}

bool SokobanSolver::isSolved() const {
    return openList->isEmpty();
}

bool SokobanSolver::isValid(State* state) const {
    int x = state->x;
    int y = state->y;
    //verificacion para jugador
    if (board->isWall(x, y)) return false; //no puede ser una pared
    if (!board->isValidPosition(x, y)) return false; //no puede salirse del tablero
    //verificacion para cajas
    for (int i = 0; i < state->numBoxes; i++) {
        for (int j = i + 1; j < state->numBoxes; j++) {
            if (state->boxX[i] == state->boxX[j] && state->boxY[i] == state->boxY[j]) {
                return false; //dos cajas en la misma posición
            }
        }
        if (board->isWall(state->boxX[i], state->boxY[i])) return false; //caja en pared
        }
    //verificacion de energia
    if (state->energia < 0) return false;
    return true;
}

bool SokobanSolver::isGoalState(State* state) {
    return state->boxesLeft == 0;
}

int SokobanSolver::getHeuristic(State* state) {
    // Heurística: suma de distancias Manhattan de cada caja a su objetivo más cercano
    int totalDistance = 0;
    for (int i = 0; i < state->numBoxes; i++) {
        int boxX = state->boxX[i];
        int boxY = state->boxY[i];
        int minDist = INT_MAX;
        for (int j = 0; j < board->numGoals; j++) {
            int goalX = board->goalX[j];
            int goalY = board->goalY[j];
            int dist = abs(boxX - goalX) + abs(boxY - goalY);
            if (dist < minDist) {
                minDist = dist;
            }
        }
        totalDistance += minDist;
    }
    return totalDistance;
}

bool SokobanSolver::isBoxPushable(State* state, int boxIndex) const {
    int boxX = state->boxX[boxIndex];
    int boxY = state->boxY[boxIndex];

    // Verificar si la caja está bloqueada por paredes o bordes del tablero
    bool blockedHorizontally = (board->isWall(boxX, boxY - 1) && board->isWall(boxX, boxY + 1));
    bool blockedVertically = (board->isWall(boxX - 1, boxY) && board->isWall(boxX + 1, boxY));

    if (blockedHorizontally && blockedVertically) {
        return false; // La caja está bloqueada y no se puede empujar
    }
    return true; // La caja es empujable
}

bool SokobanSolver::boxOnGoal(State* state, int boxIndex) const {
    int boxX = state->boxX[boxIndex];
    int boxY = state->boxY[boxIndex];
    return board->isGoal(boxX, boxY);
}

int SokobanSolver::getAdvancedHeuristic(State* state) {
    // Heurística avanzada: combina la distancia Manhattan con el número de cajas no colocadas
    int manhattanDistance = getHeuristic(state);
    int boxesNotOnGoal = state->boxesLeft;
    return manhattanDistance + (boxesNotOnGoal * 10); // Peso adicional por cajas no colocadas
}

int SokobanSolver::getEvenBetterHeuristic(State* state) {
    int total = 0;
    
    for (int i = 0; i < state->numBoxes; i++) {
        // Verificar si la caja ya está en objetivo
        bool onGoal = false;
        for (int j = 0; j < board->numGoals; j++) {
            if (state->boxX[i] == board->goalX[j] && state->boxY[i] == board->goalY[j]) {
                onGoal = true;
                break;
            }
        }
        
        if (!onGoal) {
            int minBoxToGoal = INT_MAX;
            // Encontrar objetivo más cercano para esta caja
            for (int j = 0; j < board->numGoals; j++) {
                int dist = abs(state->boxX[i] - board->goalX[j]) + 
                           abs(state->boxY[i] - board->goalY[j]);
                if (dist < minBoxToGoal) {
                    minBoxToGoal = dist;
                }
            }
            
            // Distancia del jugador a la caja
            int playerToBox = abs(state->x - state->boxX[i]) + 
                              abs(state->y - state->boxY[i]);
            
            // Combinar: jugador debe llegar a la caja + caja a objetivo
            total += minBoxToGoal + playerToBox;
        }
    }
    return total;
}

int SokobanSolver::getOptimizedMultiBoxHeuristic(State* state) {
    if (state->boxesLeft == 0) return 0;
    int total = 0;
    int minPlayerToBox = INT_MAX;
    
    // 1. Calcular distancia jugador a caja más cercana que necesita moverse
    for (int i = 0; i < state->numBoxes; i++) {
        bool onGoal = false;
        for (int j = 0; j < board->numGoals; j++) {
            if (state->boxX[i] == board->goalX[j] && state->boxY[i] == board->goalY[j]) {
                onGoal = true;
                break;
            }
        }
        if (!onGoal) {
            int playerToBox = abs(state->x - state->boxX[i]) + 
                              abs(state->y - state->boxY[i]);
            if (playerToBox < minPlayerToBox) {
                minPlayerToBox = playerToBox;
            }
        }
    }
    // 2. Calcular distancia mínima de cada caja a objetivo libre
    for (int i = 0; i < state->numBoxes; i++) {
        bool onGoal = false;
        for (int j = 0; j < board->numGoals; j++) {
            if (state->boxX[i] == board->goalX[j] && state->boxY[i] == board->goalY[j]) {
                onGoal = true;
                break;
            }
        }
        if (!onGoal) {
            int minBoxToGoal = INT_MAX;
            for (int j = 0; j < board->numGoals; j++) {
                // Verificar si el objetivo está libre
                bool goalFree = true;
                for (int k = 0; k < state->numBoxes; k++) {
                    if (k != i && state->boxX[k] == board->goalX[j] && 
                        state->boxY[k] == board->goalY[j]) {
                        goalFree = false;
                        break;
                    }
                }
                if (goalFree) {
                    int dist = abs(state->boxX[i] - board->goalX[j]) + 
                               abs(state->boxY[i] - board->goalY[j]);
                    if (dist < minBoxToGoal) minBoxToGoal = dist;
                }
            }
            if (minBoxToGoal == INT_MAX) minBoxToGoal = 0; // Todos objetivos ocupados
            total += minBoxToGoal;
        }
    }
    
    // 3. Combinar con peso estratégico
    if (minPlayerToBox == INT_MAX) minPlayerToBox = 0; // Todas las cajas en objetivo
    
    // Ponderación: priorizar acercarse a cajas sobre moverlas (2:1 ratio)
    return minPlayerToBox * 2 + total;
}

int SokobanSolver::getDirectionAwareHeuristic(State* state) {
    if (state->boxesLeft == 0) return 0;
    
    int total = 0;
    int minPlayerToBox = INT_MAX;
    
    for (int i = 0; i < state->numBoxes; i++) {
        bool onGoal = false;
        for (int j = 0; j < board->numGoals; j++) {
            if (state->boxX[i] == board->goalX[j] && state->boxY[i] == board->goalY[j]) {
                onGoal = true;
                break;
            }
        }
        
        if (!onGoal) {
            // 1. Distancia caja-objetivo más cercano
            int minBoxToGoal = INT_MAX;
            int targetGoalX = -1, targetGoalY = -1;
            
            for (int j = 0; j < board->numGoals; j++) {
                int dist = abs(state->boxX[i] - board->goalX[j]) + 
                           abs(state->boxY[i] - board->goalY[j]);
                if (dist < minBoxToGoal) {
                    minBoxToGoal = dist;
                    targetGoalX = board->goalX[j];
                    targetGoalY = board->goalY[j];
                }
            }
            
            // 2. PENALIZACIÓN CRÍTICA: Movimientos laterales cuando se necesita movimiento vertical
            int directionalPenalty = 0;
            
            // Si el objetivo está ARRIBA de la caja, penalizar movimientos horizontales
            if (targetGoalX < state->boxX[i]) { // Objetivo arriba
                // Penalizar si la caja se mueve horizontalmente lejos del camino vertical al objetivo
                int horizontalDeviation = abs(state->boxY[i] - targetGoalY);
                directionalPenalty += horizontalDeviation * 5; // Fuerte penalización
                
                // Penalizar adicionalmente si la caja está en la misma columna que el objetivo pero no sube
                if (state->boxY[i] == targetGoalY) {
                    // Debería estar subiendo, no moviéndose lateralmente
                    int verticalDistance = state->boxX[i] - targetGoalX;
                    directionalPenalty += verticalDistance * 3;
                }
            }
            
            // 3. Distancia jugador-caja
            int playerToBox = abs(state->x - state->boxX[i]) + 
                              abs(state->y - state->boxY[i]);
            
            if (playerToBox < minPlayerToBox) {
                minPlayerToBox = playerToBox;
            }
            
            total += (minBoxToGoal * 3) + directionalPenalty;
        }
    }
    
    // 4. PENALIZACIÓN GLOBAL: Evitar que las cajas se alejen de la fila de objetivos
    int globalPenalty = 0;
    for (int i = 0; i < state->numBoxes; i++) {
        // Las cajas deberían estar en las filas 1-2-3 (cerca de objetivos), no en la fila 3+ abajo
        if (state->boxX[i] > 3) { // Demasiado abajo
            globalPenalty += (state->boxX[i] - 3) * 10;
        }
        
        // Penalizar cajas que están en columnas incorrectas (lejos de los objetivos 3 y 8)
        if (state->boxY[i] < 2 || state->boxY[i] > 9) { // Muy a los extremos
            globalPenalty += 5;
        }
    }
    
    if (minPlayerToBox == INT_MAX) minPlayerToBox = 0;
    
    return total + minPlayerToBox * 2 + globalPenalty;
}
int SokobanSolver::getAggressiveDirectionHeuristic(State* state) {
    if (state->boxesLeft == 0) return 0;
    
    int total = 0;
    int minPlayerToBox = INT_MAX;
    
    for (int i = 0; i < state->numBoxes; i++) {
        bool onGoal = false;
        for (int j = 0; j < board->numGoals; j++) {
            if (state->boxX[i] == board->goalX[j] && state->boxY[i] == board->goalY[j]) {
                onGoal = true;
                break;
            }
        }
        
        if (!onGoal) {
            // 1. Distancia caja-objetivo más cercano
            int minBoxToGoal = INT_MAX;
            int targetGoalX = -1, targetGoalY = -1;
            
            for (int j = 0; j < board->numGoals; j++) {
                int dist = abs(state->boxX[i] - board->goalX[j]) + 
                           abs(state->boxY[i] - board->goalY[j]);
                if (dist < minBoxToGoal) {
                    minBoxToGoal = dist;
                    targetGoalX = board->goalX[j];
                    targetGoalY = board->goalY[j];
                }
            }
            
            // 2. PENALIZACIÓN HIPER-AGRESIVA: Movimientos laterales vs verticales
            int directionalPenalty = 0;
            
            // CRÍTICO: Si el objetivo está ARRIBA, penalizar MUCHO los movimientos horizontales
            if (targetGoalX < state->boxX[i]) { // Objetivo arriba
                // PENALIZACIÓN MASIVA por desviación horizontal
                int horizontalDeviation = abs(state->boxY[i] - targetGoalY);
                directionalPenalty += horizontalDeviation * 25; // Aumentado drasticamente
                
                // PENALIZACIÓN EXTRA si está en columna incorrecta
                if (state->boxY[i] != targetGoalY) {
                    directionalPenalty += 30;
                }
                
                // BONIFICACIÓN por progreso vertical
                int verticalProgress = (state->boxX[i] - targetGoalX);
                directionalPenalty -= verticalProgress * 8; // Recompensar subir
                
                // PENALIZACIÓN EXTRA por estar en filas bajas
                if (state->boxX[i] >= 3) {
                    directionalPenalty += (state->boxX[i] - 2) * 20;
                }
            }
            
            // 3. Distancia jugador-caja con peso estratégico
            int playerToBox = abs(state->x - state->boxX[i]) + 
                              abs(state->y - state->boxY[i]);
            
            // Peso mayor si la caja necesita moverse verticalmente
            if (targetGoalX < state->boxX[i]) {
                playerToBox = playerToBox * 2; // Doble importancia
            }
            
            if (playerToBox < minPlayerToBox) {
                minPlayerToBox = playerToBox;
            }
            
            total += (minBoxToGoal * 4) + directionalPenalty;
        } else {
            // Caja en objetivo - gran recompensa
            total -= 50;
        }
    }
    
    // 4. PENALIZACIÓN GLOBAL MEJORADA
    int globalPenalty = 0;
    
    // Penalizar MUCHO cajas en filas bajas
    for (int i = 0; i < state->numBoxes; i++) {
        if (state->boxX[i] > 2) { // Filas 3+ 
            globalPenalty += (state->boxX[i] - 1) * 25; // Penalización masiva
        }
        
        // Penalizar cajas en posiciones extremas
        if (state->boxY[i] < 2 || state->boxY[i] > 9) {
            globalPenalty += 15;
        }
    }
    
    // 5. BONIFICACIÓN por cajas cerca de objetivos verticalmente
    for (int i = 0; i < state->numBoxes; i++) {
        bool onGoal = false;
        for (int j = 0; j < board->numGoals; j++) {
            if (state->boxX[i] == board->goalX[j] && state->boxY[i] == board->goalY[j]) {
                onGoal = true;
                break;
            }
        }
        
        if (!onGoal) {
            // Bonificar si está en fila 1 o 2 (cerca verticalmente)
            if (state->boxX[i] <= 2) {
                globalPenalty -= 20; // Gran recompensa
            }
            
            // Bonificar si está en columna correcta
            for (int j = 0; j < board->numGoals; j++) {
                if (state->boxY[i] == board->goalY[j]) {
                    globalPenalty -= 10; // Recompensa por columna correcta
                    break;
                }
            }
        }
    }
    
    if (minPlayerToBox == INT_MAX) minPlayerToBox = 0;
    
    return total + minPlayerToBox * 3 + globalPenalty;
}

int SokobanSolver::getFacil6NuclearHeuristic(State* state) {
    if (state->boxesLeft == 0) return -1000; // Gran recompensa por solución
    
    int total = 0;
    
    // Objetivos específicos de Facil-6
    const int goal1X = 1, goal1Y = 3;
    const int goal2X = 1, goal2Y = 8;
    
    // Asignación óptima basada en distancia
    int dist1 = abs(state->boxX[0] - goal1X) + abs(state->boxY[0] - goal1Y) +
                abs(state->boxX[1] - goal2X) + abs(state->boxY[1] - goal2Y);
    
    int dist2 = abs(state->boxX[0] - goal2X) + abs(state->boxY[0] - goal2Y) +
                abs(state->boxX[1] - goal1X) + abs(state->boxY[1] - goal1Y);
    
    int minDist = min(dist1, dist2);
    
    // PENALIZACIÓN HIPER-AGRESIVA POR MOVIMIENTOS HORIZONTALES
    int horizontalPenalty = 0;
    
    for (int i = 0; i < state->numBoxes; i++) {
        int targetGoalX = (minDist == dist1 && i == 0) ? goal1X : 
                        (minDist == dist1 && i == 1) ? goal2X :
                        (minDist == dist2 && i == 0) ? goal2X : goal1X;
        
        int targetGoalY = (minDist == dist1 && i == 0) ? goal1Y : 
                        (minDist == dist1 && i == 1) ? goal2Y :
                        (minDist == dist2 && i == 0) ? goal2Y : goal1Y;
        
        // PENALIZACIÓN MASIVA por cualquier movimiento horizontal
        int horizontalDeviation = abs(state->boxY[i] - targetGoalY);
        horizontalPenalty += horizontalDeviation * 40; // Penalización extrema
        
        // PENALIZACIÓN por no estar subiendo
        if (state->boxX[i] > targetGoalX) {
            int verticalStagnation = state->boxX[i] - targetGoalX;
            horizontalPenalty += verticalStagnation * 20;
        }
        
        // BONIFICACIÓN MASIVA por progreso vertical
        if (state->boxX[i] < 3) { // En filas 0,1,2
            horizontalPenalty -= (3 - state->boxX[i]) * 25;
        }
        
        // BONIFICACIÓN por estar en columna objetivo
        if (state->boxY[i] == targetGoalY) {
            horizontalPenalty -= 20;
        }
    }
    
    // Distancia jugador a caja más cercana
    int playerToBox1 = abs(state->x - state->boxX[0]) + abs(state->y - state->boxY[0]);
    int playerToBox2 = abs(state->x - state->boxX[1]) + abs(state->y - state->boxY[1]);
    int minPlayerToBox = min(playerToBox1, playerToBox2);
    
    return minDist * 5 + horizontalPenalty + minPlayerToBox * 2;
}