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
            // chequeo: si la caja está bloqueada, el jugador debe llevar la llave correspondiente
            char locked = s->lockedBoxesChar[i];
            if (locked != '\0') {
                // llave correspondiente es la minúscula del char
                char needed = (char)(locked - 'A' + 'a'); // restar y sumar ASCII
                if (s->currentKey != needed) {
                    // no puede empujar la caja bloqueada sin la llave
                    return false;
                }
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
            pushedBox = true;
            pushedBoxIndex = i;
            char locked = newState->lockedBoxesChar[pushedBoxIndex];
            if (locked == '\0') {
                newState->boxX[i] += dx;
                newState->boxY[i] += dy;
            }
            if (locked != '\0' && newState->currentKey != 0) {
                char needed = (char)(locked - 'A' + 'a');
                if (newState->currentKey == needed) {
                    newState->lockedBoxesChar[pushedBoxIndex] = '\0'; // desbloqueada
                    newState->currentKey = 0; // llave consumida
                    newState->boxX[i] += dx;
                    newState->boxY[i] += dy;
                }
            }
            break; // solo una caja puede estar allí
        }
    }
    if (!pushedBox) {
        // buscar llave en lista de llaves del estado
        if (newState->currentKey == 0 && newState->numKeys > 0) {
            int pickIdx = -1;
            for (int k = 0; k < newState->numKeys; ++k) {
                if (newState->keyX[k] == newState->x && newState->keyY[k] == newState->y) { pickIdx = k; break; }
            }
            if (pickIdx >= 0) {
                // recoger llave
                newState->currentKey = newState->keyChar[pickIdx];
                // eliminar llave del arreglo (compactar)
                int nk = newState->numKeys - 1;
                for (int t = pickIdx; t < nk; ++t) {
                    newState->keyX[t] = newState->keyX[t+1];
                    newState->keyY[t] = newState->keyY[t+1];
                    newState->keyChar[t] = newState->keyChar[t+1];
                }
                // reducir numero de llaves
                if (nk == 0) {
                    delete[] newState->keyX; delete[] newState->keyY; delete[] newState->keyChar;
                    newState->keyX = nullptr; newState->keyY = nullptr; newState->keyChar = nullptr;
                }
                newState->numKeys = nk;
            }
        }
    }

    newState->lastMove = getMoveChar();
    newState->parent = s;

    // canonicalizar representación de cajas
    newState->canonicalize();
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