#ifndef OPERATION_H
#define OPERATION_H

#include "State.h"
#include "Board.h"

class Operation {
private:
    int dx, dy; // dx = cambio en fila, dy = cambio en columna
public:
    Operation(int dx, int dy);
    State *execute(State *s) const;
    char getMoveChar() const;
    bool isPush(const State *s) const;
    bool isPushedToGoal(const State *s, const Board *board) const;
    bool isPushedOutOfGoal(const State *s, const Board *board) const;
    bool canExecute(const State *s, const Board *board) const;
};

#endif