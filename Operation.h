#ifndef OPERATION_H
#define OPERATION_H

#include "State.h"
#include "Board.h"

class Operation {
private:
    int dx, dy; // dx = cambio en fila, dy = cambio en columna
public:
    Operation(int dx, int dy);
    State *execute(State *s) const;          // clona y aplica el movimiento (asume canExecute ya verificado)
    char getMoveChar() const;
    bool isPush(const State *s) const;       // si el movimiento empuja una caja
    bool isPushedToGoal(const State *s, const Board *board) const;
    bool isPushedOutOfGoal(const State *s, const Board *board) const;

    // Nuevo: verifica si la operacion es ejecutable (no sale del tablero, no empuja muro, no empuja caja sobre otra caja)
    bool canExecute(const State *s, const Board *board) const;
};

#endif
