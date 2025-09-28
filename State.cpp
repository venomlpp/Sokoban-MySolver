#include "State.h"
#include <algorithm>

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
    parent = nullptr; // no copiar parent por seguridad
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

bool State::equals(const State* other) const {
    if (x != other->x || y != other->y || 
        numBoxes != other->numBoxes || boxesLeft != other->boxesLeft || energia != other->energia) {
        return false;
    }
    // Suponiendo canonicalize() fue llamado: comparar arrays directamente
    for (int i = 0; i < numBoxes; i++) {
        if (boxX[i] != other->boxX[i] || boxY[i] != other->boxY[i]) return false;
    }
    return true;
}

State *State::clone() {
    return new State(*this);
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
    if (lastMove != ' ') cout << lastMove << " ";
}

void State::canonicalize() {
    // Orden simple O(n^2) por (x,y) (suficiente para pocos boxes)
    for (int i = 0; i < numBoxes - 1; ++i) {
        for (int j = i + 1; j < numBoxes; ++j) {
            if (boxX[i] > boxX[j] || (boxX[i] == boxX[j] && boxY[i] > boxY[j])) {
                std::swap(boxX[i], boxX[j]);
                std::swap(boxY[i], boxY[j]);
            }
        }
    }
}

int State::getHeuristic(State *state) const {
    // placeholder: implementación externa en SokobanSolver
    return 0;
}
