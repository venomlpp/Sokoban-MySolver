// implementación de la clase State en State.h
#include "State.h"

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
}

State::~State() {
    delete[] boxX;
    delete[] boxY;
}

State::State(const State &other) { //puede desaparecer
    x = other.x;
    y = other.y;
    costo = other.costo;
    heuristic = other.heuristic;
    energia = other.energia;
    parent = other.parent;
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
    // Comparación rápida de atributos básicos
    if (x != other->x || y != other->y || 
        numBoxes != other->numBoxes || boxesLeft != other->boxesLeft) {
        return false;
    }
    // Comparar cajas (debe ser independiente del orden)
    for (int i = 0; i < numBoxes; i++) {
        bool found = false;
        for (int j = 0; j < other->numBoxes; j++) {
            if (boxX[i] == other->boxX[j] && boxY[i] == other->boxY[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
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
        if (i < numBoxes - 1) {
            cout << ", ";
        }
    }
    cout << "\n";
    cout << "Ultimo movimiento: " << lastMove << "\n";
}

void State::printPath() const {
    if (parent != nullptr) {
        parent->printPath();
    }
    if (lastMove != ' ') {
        cout << lastMove << " ";
    }
}