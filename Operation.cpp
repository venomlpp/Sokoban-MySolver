#include "Operation.h"
#include <iostream>

Operation::Operation(int dx, int dy) : dx(dx), dy(dy) {}

char Operation::getMoveChar() const {
    // Consistencia: dx = cambio de fila, dy = cambio de columna
    if (dx == -1 && dy == 0) return 'U';
    if (dx == 1  && dy == 0) return 'D';
    if (dx == 0  && dy == -1) return 'L';
    if (dx == 0  && dy == 1) return 'R';
    return '?';
}

State *Operation::execute(State *s) const {
    // Nota: llamador debe haber verificado canExecute antes de llamar a execute.
    State *newState = s->clone();

    newState->x += dx;
    newState->y += dy;

    // vemos si mueve una caja
    bool pushedBox = false;
    int pushedBoxIndex = -1;
    for (int i = 0; i < newState->numBoxes; i++) {
        if (newState->boxX[i] == newState->x && newState->boxY[i] == newState->y) {
            newState->boxX[i] += dx;
            newState->boxY[i] += dy;
            pushedBox = true;
            pushedBoxIndex = i;
        }
    }
    newState->lastMove = getMoveChar();
    newState->parent = s;

    // canonicalizar para que la representación sea estable (útil para hashing)
    newState->canonicalize();

    // Debug logging - opcional
    if (pushedBox) {
        cout << "EMPUJE: Jugador (" << s->x << "," << s->y << ") -> (" << newState->x << "," << newState->y << ")"
             << ", Caja " << pushedBoxIndex << " -> (" << newState->boxX[pushedBoxIndex] << "," << newState->boxY[pushedBoxIndex] << ")" << endl;
    }
    return newState;
}

bool Operation::isPush(const State *s) const {
    int newX = s->x + dx;
    int newY = s->y + dy;

    for (int i = 0; i < s->numBoxes; i++) {
        if (s->boxX[i] == newX && s->boxY[i] == newY) {
            return true; // Está empujando una caja
        }
    }
    return false; // No está empujando ninguna caja
}

bool Operation::isPushedToGoal(const State *s, const Board *board) const {
    int newX = s->x + dx;
    int newY = s->y + dy;

    for (int i = 0; i < s->numBoxes; i++) {
        if (s->boxX[i] == newX && s->boxY[i] == newY) {
            int boxNewX = s->boxX[i] + dx;
            int boxNewY = s->boxY[i] + dy;
            return board->isGoal(boxNewX, boxNewY);
        }
    }
    return false; // No está empujando ninguna caja
}

bool Operation::isPushedOutOfGoal(const State *s, const Board *board) const {
    int newX = s->x + dx;
    int newY = s->y + dy;

    for (int i = 0; i < s->numBoxes; i++) {
        if (s->boxX[i] == newX && s->boxY[i] == newY) {
            if (!board->isGoal(s->boxX[i], s->boxY[i])) {
                return false; // La caja no estaba en un objetivo antes de ser empujada
            }
            int boxNewX = s->boxX[i] + dx;
            int boxNewY = s->boxY[i] + dy;
            return !board->isGoal(boxNewX, boxNewY);
        }
    }
    return false; // No salió de un objetivo
}

bool Operation::canPush(const State *s, const Board *board) const {
    // Verifica si hay caja en la casilla objetivo y si su destino es válido
    int newX = s->x + dx;
    int newY = s->y + dy;
    for (int i = 0; i < s->numBoxes; i++) {
        if (s->boxX[i] == newX && s->boxY[i] == newY) {
            int destX = newX + dx;
            int destY = newY + dy;
            // destino dentro y no pared
            if (!board->isValidPosition(destX, destY) || board->isWall(destX, destY)) return false;
            // destino no ocupado por otra caja
            for (int j = 0; j < s->numBoxes; j++) {
                if (j != i && s->boxX[j] == destX && s->boxY[j] == destY) return false;
            }
            return true;
        }
    }
    // No hay caja => nothing to push
    return true;
}

bool Operation::canExecute(const State *s, const Board *board) const {
    int newX = s->x + dx;
    int newY = s->y + dy;
    // nueva celda válida y no pared
    if (!board->isValidPosition(newX, newY) || board->isWall(newX, newY)) return false;
    // si hay caja, comprobar si el empuje es válido
    for (int i = 0; i < s->numBoxes; i++) {
        if (s->boxX[i] == newX && s->boxY[i] == newY) {
            return canPush(s, board);
        }
    }
    return true;
}
