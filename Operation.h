#ifndef OPERATION_H
#define OPERATION_H

#include "State.h"
#include "Board.h"

class Operation {
private:
    int dx, dy; // dx = cambio en fila (row), dy = cambio en columna (col)
public:
    Operation(int dx, int dy);
    State *execute(State *s) const; // clona y aplica el movimiento (no verifica validez)
    char getMoveChar() const;
    bool isPush(const State *s) const;
    bool isPushedToGoal(const State *s, const Board *board) const;
    bool isPushedOutOfGoal(const State *s, const Board *board) const;

    // NUEVO: verifica si la operación es ejecutable (movimiento válido o empuje válido)
    bool canExecute(const State *s, const Board *board) const;
    bool canPush(const State *s, const Board *board) const;
};

#endif
