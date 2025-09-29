#ifndef STATE_H
#define STATE_H

#include <iostream>
using namespace std;

class State {
public:
    int x, y;
    int costo;
    int heuristic;
    int energia;
    int f_cost;
    State* parent;
    char lastMove;
    int *boxX;
    int *boxY;
    int numBoxes;
    int boxesLeft;

    State(int x, int y, int *boxX, int *boxY, int numBoxes, int boxesLeft, int energia);
    State(const State &other);
    ~State();

    bool equals(const State *other) const;
    void printState() const;
    void printPath() const;
    State *clone();

    int getHeuristic(State *state) const;

    // NUEVO: ordena las cajas por (x,y) para una representación canónica
    void canonicalize();
};

#endif
