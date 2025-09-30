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

State::State(int x, int y, int *boxX, int *boxY, char *lockedBoxesChar, int numBoxes, 
        int boxesLeft, int *keyX, int *keyY, char *keyChar, int numKeys, int energia) {
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
    this->lockedBoxesChar = new char[numBoxes];
    for (int i = 0; i < numBoxes; i++) {
        this->boxX[i] = boxX[i];
        this->boxY[i] = boxY[i];
        this->lockedBoxesChar[i] = (lockedBoxesChar ? lockedBoxesChar[i] : '\0');
    }
    this->numKeys = numKeys;
    if (numKeys > 0) {
        this->keyX = new int[numKeys];
        this->keyY = new int[numKeys];
        this->keyChar = new char[numKeys];
        for (int k = 0; k < numKeys; k++) {
            this->keyX[k] = keyX[k];
            this->keyY[k] = keyY[k];
            this->keyChar[k] = keyChar[k];
        }
    } else {
        this->keyX = nullptr;
        this->keyY = nullptr;
        this->keyChar = nullptr;
    }
    this->currentKey = 0; // no lleva llave al inicio
    canonicalize();
}

State::~State() {
    delete[] boxX;
    delete[] boxY;
    delete[] lockedBoxesChar;
    delete[] keyX;
    delete[] keyY;
    delete[] keyChar;
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
    lockedBoxesChar = new char[numBoxes];
    for (int i = 0; i < numBoxes; i++) {
        boxX[i] = other.boxX[i];
        boxY[i] = other.boxY[i];
        lockedBoxesChar[i] = other.lockedBoxesChar[i];
    }
    numKeys = other.numKeys;
    if (numKeys > 0) {
        keyX = new int[numKeys];
        keyY = new int[numKeys];
        keyChar = new char[numKeys];
        for (int k = 0; k < numKeys; k++) {
            keyX[k] = other.keyX[k];
            keyY[k] = other.keyY[k];
            keyChar[k] = other.keyChar[k];
        }
    } else {
        keyX = nullptr;
        keyY = nullptr;
        keyChar = nullptr;
    }

    currentKey = other.currentKey;
}

State *State::clone() {
    return new State(*this);
}

void State::canonicalize() { //ahora incluye llaves y cajas bloqueadas
    // ordena cajas por (x,y) y mantiene lockedBoxesChar emparejado
    for (int i = 0; i < numBoxes; ++i) {
        int best = i;
        for (int j = i + 1; j < numBoxes; ++j) {
            if (boxX[j] < boxX[best] || (boxX[j] == boxX[best] && boxY[j] < boxY[best])) {
                best = j;
            }
        }
        if (best != i) {
            int tx = boxX[i], ty = boxY[i];
            char tc = lockedBoxesChar[i];
            boxX[i] = boxX[best]; boxY[i] = boxY[best]; lockedBoxesChar[i] = lockedBoxesChar[best];
            boxX[best] = tx; boxY[best] = ty; lockedBoxesChar[best] = tc;
        }
    }
    if (numKeys > 1) {
        for (int i = 0; i < numKeys; ++i) {
            int best = i;
            for (int j = i + 1; j < numKeys; ++j) {
                if (keyX[j] < keyX[best] || (keyX[j] == keyX[best] && keyY[j] < keyY[best])) best = j;
            }
            if (best != i) {
                int tx = keyX[i], ty = keyY[i]; char tc = keyChar[i];
                keyX[i] = keyX[best]; keyY[i] = keyY[best]; keyChar[i] = keyChar[best];
                keyX[best] = tx; keyY[best] = ty; keyChar[best] = tc;
            }
        }
    }
}

bool State::equals(const State* other) const {
    if (x != other->x || y != other->y) return false;
    if (numBoxes != other->numBoxes) return false;
    if (boxesLeft != other->boxesLeft) return false;
    if (currentKey != other->currentKey) return false;
    for (int i = 0; i < numBoxes; ++i) {
        if (boxX[i] != other->boxX[i] || boxY[i] != other->boxY[i]) return false;
        if (lockedBoxesChar[i] != other->lockedBoxesChar[i]) return false;
    }
    if (numKeys != other->numKeys) return false;
    for (int k = 0; k < numKeys; ++k) {
        if (keyX[k] != other->keyX[k] || keyY[k] != other->keyY[k] || keyChar[k] != other->keyChar[k]) return false;
    }
    return true;
}

void State::printState() const {
    cout << "Posicion jugador: (" << x << ", " << y << "), ";
    cout << "Energia: " << energia << ", Lleva llave: " << (currentKey ? currentKey : '-') << "\n";
    cout << "Cajas: ";
    for (int i = 0; i < numBoxes; i++) {
        cout << "  Caja " << i + 1 << ": (" << boxX[i] << ", " << boxY[i] << ")";
        if (lockedBoxesChar[i]) cout << "[" << lockedBoxesChar[i] << "]";
        if (i < numBoxes - 1) cout << ", ";
    }
    cout << "\n";
    cout << "Llaves en suelo: ";
    for (int k = 0; k < numKeys; ++k) {
        cout << "(" << keyX[k] << "," << keyY[k] << ":" << keyChar[k] << ")";
        if (k < numKeys-1) cout << ", ";
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