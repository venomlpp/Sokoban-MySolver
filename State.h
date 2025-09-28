#ifndef STATE_H
#define STATE_H

#include <iostream>
using namespace std;

class State {
public:
    int x, y; // coordenadas del jugador: x = fila (row), y = columna (col)
    int costo; // costo del camino (g)
    int heuristic; // valor heurístico (h)
    int energia;
    int f_cost; // costo + heuristic
    State* parent; //puntero al estado padre
    char lastMove; //último movimiento realizado para llegar a este estado
    int *boxX; //arreglo dinámico con las coordenadas x (fila) de las cajas
    int *boxY; //arreglo dinámico con las coordenadas y (col) de las cajas
    int numBoxes; //número de cajas en el juego
    int boxesLeft; //número de cajas que faltan por colocar en los objetivos

    State(int x, int y, int *boxX, int *boxY, int numBoxes, int boxesLeft, int energia);
    State(const State &other);
    ~State();

    bool equals(const State *other) const;
    void printState() const;
    void printPath() const;
    State *clone();
    void canonicalize(); // garantiza orden de cajas (representación canónica)
    int getHeuristic(State *state) const;
};

#endif
