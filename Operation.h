#ifndef OPERATION_H
#define OPERATION_H

#include "State.h"
#include "Board.h"

class Operation {
    private:
    int dx, dy;
    public:
    Operation(int dx, int dy);
    State *execute(State *s) const;
    char getMoveChar() const;
    bool isPush(const State *s);
    bool isPushedToGoal(const State *s, const Board *board);
    bool isPushedOutOfGoal(const State *s, const Board *board);
};
#endif