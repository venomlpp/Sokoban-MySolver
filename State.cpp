#include "State.h"
#include <cstring>

State::State(int x, int y, int *boxX, int *boxY, int numBoxes, int boxesLeft, int energia) {
    this->x = x;
    this->y = y;
    this->costo = 0;
    this->heuristic = 0;
    this->energia = energia;
    this->parent = nullptr;
    this->lastMove = ' ';
    this->numBoxes = numBoxes;
    this->boxesLeft = boxesLeft;

    this->boxX = new int[numBoxes];
    this->boxY = new int[numBoxes];
    for (int i = 0; i < numBoxes; i++) {
        this->boxX[i] = boxX[i];
        this->boxY[i] = boxY[i];
    }
    canonicalize();
}

State::~State() {
    delete[] boxX;
    delete[] boxY;
}

State::State(const State &other) {
    x = other.x;
    y = other.y;
    costo = other.costo;
    heuristic = other.heuristic;
    energia = other.energia;
    parent = nullptr; // no copiar parent
    lastMove = other.lastMove;
    numBoxes = other.numBoxes;
    boxesLeft = other.boxesLeft;

    boxX = new int[numBoxes];
    boxY = new int[numBoxes];
    for (int i = 0; i < numBoxes; i++) {
        boxX[i] = other.boxX[i];
        boxY[i] = other.boxY[i];
    }
}

State *State::clone() {
    return new State(*this);
}

void State::canonicalize() {
    for (int i = 0; i < numBoxes; ++i) {
        int best = i;
        for (int j = i + 1; j < numBoxes; ++j) {
            if (boxX[j] < boxX[best] || (boxX[j] == boxX[best] && boxY[j] < boxY[best])) {
                best = j;
            }
        }
        if (best != i) {
            int tx = boxX[i], ty = boxY[i];
            boxX[i] = boxX[best]; boxY[i] = boxY[best];
            boxX[best] = tx; boxY[best] = ty;
        }
    }
}

bool State::equals(const State* other) const {
    if (x != other->x || y != other->y) return false;
    if (numBoxes != other->numBoxes) return false;
    for (int i = 0; i < numBoxes; ++i) {
        if (boxX[i] != other->boxX[i] || boxY[i] != other->boxY[i]) return false;
    }
    return true;
}

void State::printState() const {
    cout << "Posicion jugador: (" << x << ", " << y << "), ";
    cout << "Energia: " << energia << "\n";
    cout << "Cajas: ";
    for (int i = 0; i < numBoxes; i++) {
        cout << "  Caja " << i + 1 << ": (" << boxX[i] << ", " << boxY[i] << ")";
        if (i < numBoxes - 1) cout << ", ";
    }
    cout << "\n";
    cout << "Ultimo movimiento: " << lastMove << "\n";
}

void State::printPath() const {
    if (parent != nullptr) parent->printPath();
    if (lastMove != ' ') cout << lastMove;
}

int State::getHeuristic(State *state) const {
    return 0;
}
