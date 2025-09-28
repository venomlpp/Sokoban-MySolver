#include "Operation.h"

Operation::Operation(int dx, int dy) : dx(dx), dy(dy) {}

State *Operation::execute(State *s) const {
    State *newState = s->clone();

    newState->x += dx;
    newState->y += dy;

    //vemos si mueve una caja
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

    // Debug logging
    if (pushedBox) {
        cout << "EMPUJE: Jugador (" << s->x << "," << s->y << ") -> (" << newState->x << "," << newState->y << ")"
             << ", Caja " << pushedBoxIndex << " -> (" << newState->boxX[pushedBoxIndex] << "," << newState->boxY[pushedBoxIndex] << ")" << endl;
    }
    return newState;
}

char Operation::getMoveChar() const {
    if (dx == -1 && dy == 0) return 'U';
    if (dx == 1 && dy == 0) return 'D';
    if (dx == 0 && dy == -1) return 'L';
    if (dx == 0 && dy == 1) return 'R';
    return '?';
}

bool Operation::isPush(const State *s) {
    int newX = s->x + dx;
    int newY = s->y + dy;

    for (int i = 0; i < s->numBoxes; i++) {
        if (s->boxX[i] == newX && s->boxY[i] == newY) {
            return true; // Está empujando una caja
        }
    }
    return false; // No está empujando ninguna caja
}

bool Operation::isPushedToGoal(const State *s, const Board *board) {
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

bool Operation::isPushedOutOfGoal(const State *s, const Board *board) {
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