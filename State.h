#ifndef STATE_H
#define STATE_H

#include <iostream>
using namespace std;


/*este será el estado, una "snapshot" del juego Sokoban en un momento dado. Incluye
Información sobre la posición del jugador, de las cajas, de la energía del jugador,
y del árbol de estados para armar un camino.
*/ 

class State {
public:
    int x, y; //coordenadas del jugador en el tablero
    int costo; //costo del estado (pendiente de definir)
    int heuristic; //valor heurístico del estado
    int energia;
    State* parent; //puntero al estado padre (para armar el camino)
    char lastMove; //último movimiento realizado para llegar a este estado
    int *boxX; //arreglo dinámico con las coordenadas x de las cajas
    int *boxY; //arreglo dinámico con las coordenadas y de las cajas
    int numBoxes; //número de cajas en el juego
    int boxesLeft; //número de cajas que faltan por colocar en los objetivos

    State(int x, int y, int *boxX, int *boxY, int numBoxes, int boxesLeft, int energia); //no incluye costo
    State(const State &other); //constructor de copia
    ~State(); //destructor

    bool equals(const State *other) const;
    void printState() const;
    void printPath() const;
    State *clone();

    int getHeuristic(State *state) const; //función heurística (pendiente de definir)
};

#endif