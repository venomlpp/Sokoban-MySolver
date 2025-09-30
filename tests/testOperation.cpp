#include "../Operation.h"
#include "../State.h"
#include "../Board.h"
#include <cassert>
#include <iostream>
using namespace std;

int main() {
    cout << "RUNNING Operation basic tests...\n";

    // Cargar tablero desde archivo real
    Board board("testBoards/mini.txt");
    board.printBoard();

    // Extraer info inicial
    int px, py;
    board.findPlayerStart(px, py);

    int* bx; int* by; char* locked;
    int numBoxes;
    board.findBoxesStart(bx, by, locked, numBoxes);

    int* kx; int* ky; char* kc; int numKeys;
    board.findKeysStart(kx, ky, kc, numKeys);

    State s0(px, py, bx, by, locked, numBoxes, numBoxes, kx, ky, kc, numKeys, 100);

    // --- Movimiento hacia abajo (D) ---
    Operation down(1, 0);
    assert(down.isPush(&s0) && "Debe ser push porque hay caja abajo");
    assert(down.canExecute(&s0, &board) && "Debe poder empujar la caja");

    State* s1 = down.execute(&s0);
    cout << "Después de ejecutar DOWN:\n";
    s1->printState();

    // --- Movimiento hacia la derecha (R) ---
    Operation right(0, 1);
    assert(!right.isPush(s1) && "R no debe empujar nada");
    assert(right.canExecute(s1, &board) && "Jugador puede moverse a la derecha");

    State* s2 = right.execute(s1);
    cout << "Después de ejecutar RIGHT:\n";
    s2->printState();

    cout << "\nAll Operation tests passed!\n";

    delete s1;
    delete s2;
    delete[] bx; delete[] by; delete[] locked;
    delete[] kx; delete[] ky; delete[] kc;

    return 0;
}
