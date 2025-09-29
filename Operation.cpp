#include "Operation.h"
#include <iostream>

Operation::Operation(int dx, int dy) : dx(dx), dy(dy) {}

char Operation::getMoveChar() const {
    if (dx == -1 && dy == 0) return 'U';
    if (dx == 1  && dy == 0) return 'D';
    if (dx == 0  && dy == -1) return 'L';
    if (dx == 0  && dy == 1) return 'R';
    return '?';
}

bool Operation::canExecute(const State *s, const Board *board) const {
    int nx = s->x + dx;
    int ny = s->y + dy;
    // fuera de tablero o pared
    if (!board->isValidPosition(nx, ny) || board->isWall(nx, ny)) return false;

    // si hay caja en nx,ny -> comprobar destino de la caja
    for (int i = 0; i < s->numBoxes; ++i) {
        if (s->boxX[i] == nx && s->boxY[i] == ny) {
            int destX = nx + dx;
            int destY = ny + dy;
            // dest fuera o pared
            if (!board->isValidPosition(destX, destY) || board->isWall(destX, destY)) return false;
            // dest con otra caja
            for (int j = 0; j < s->numBoxes; ++j) {
                if (j != i && s->boxX[j] == destX && s->boxY[j] == destY) return false;
            }
            // todo ok para empujar
            return true;
        }
    }
    // no hay caja, y la celda es caminable
    return true;
}

State *Operation::execute(State *s) const {
    // caller debe haber verificado canExecute antes de llamar
    State *newState = s->clone();

    newState->x += dx;
    newState->y += dy;

    bool pushedBox = false;
    int pushedBoxIndex = -1;
    for (int i = 0; i < newState->numBoxes; i++) {
        if (newState->boxX[i] == newState->x && newState->boxY[i] == newState->y) {
            newState->boxX[i] += dx;
            newState->boxY[i] += dy;
            pushedBox = true;
            pushedBoxIndex = i;
            break; // solo una caja puede estar allí
        }
    }

    newState->lastMove = getMoveChar();
    newState->parent = s;

    // canonicalizar representación de cajas
    newState->canonicalize();

    /*if (pushedBox) {
        cout << "EMPUJE: Jugador (" << s->x << "," << s->y << ") -> (" << newState->x << "," << newState->y << ")"
             << ", Caja " << pushedBoxIndex << " -> (" << newState->boxX[pushedBoxIndex] << "," << newState->boxY[pushedBoxIndex] << ")" << endl;
    }*/
    return newState;
}

bool Operation::isPush(const State *s) const {
    int nx = s->x + dx;
    int ny = s->y + dy;
    for (int i = 0; i < s->numBoxes; ++i) {
        if (s->boxX[i] == nx && s->boxY[i] == ny) return true;
    }
    return false;
}

bool Operation::isPushedToGoal(const State *s, const Board *board) const {
    int nx = s->x + dx;
    int ny = s->y + dy;
    for (int i = 0; i < s->numBoxes; ++i) {
        if (s->boxX[i] == nx && s->boxY[i] == ny) {
            int bx = s->boxX[i] + dx;
            int by = s->boxY[i] + dy;
            return board->isGoal(bx, by);
        }
    }
    return false;
}

bool Operation::isPushedOutOfGoal(const State *s, const Board *board) const {
    int nx = s->x + dx;
    int ny = s->y + dy;
    for (int i = 0; i < s->numBoxes; ++i) {
        if (s->boxX[i] == nx && s->boxY[i] == ny) {
            // si estaba en goal antes -> true si ahora no está en goal
            if (!board->isGoal(s->boxX[i], s->boxY[i])) return false;
            int bx = s->boxX[i] + dx;
            int by = s->boxY[i] + dy;
            return !board->isGoal(bx, by);
        }
    }
    return false;
}