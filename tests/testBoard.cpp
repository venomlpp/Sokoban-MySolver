#include "../Board.h"
#include <cassert>
#include <iostream>
using namespace std;

int main() {
    cout << "RUNNING Board basic tests...\n";

    // Cargar tablero desde archivo de prueba
    Board board("testBoards/mini.txt");

    // Info general
    board.printInfo();
    board.printBoard();

    // Verificar dimensiones
    assert(board.isValidPosition(0,0));
    assert(!board.isValidPosition(-1,0));

    // Buscar jugador inicial
    int px, py;
    board.findPlayerStart(px, py);
    cout << "Jugador inicial en: (" << px << "," << py << ")\n";
    assert(px >= 0 && py >= 0);

    // Buscar cajas
    int *bx, *by; char *locked;
    int numBoxes;
    board.findBoxesStart(bx, by, locked, numBoxes);
    cout << "Encontradas " << numBoxes << " cajas\n";
    for (int i = 0; i < numBoxes; i++) {
        cout << "Caja " << i << " en (" << bx[i] << "," << by[i] << ")";
        if (locked[i] != '\0') cout << " bloqueada con " << locked[i];
        cout << "\n";
    }
    assert(numBoxes > 0);

    // Buscar llaves
    int *kx, *ky; char *kc;
    int numKeys;
    board.findKeysStart(kx, ky, kc, numKeys);
    cout << "Encontradas " << numKeys << " llaves\n";
    for (int i = 0; i < numKeys; i++) {
        cout << "Llave " << i << " en (" << kx[i] << "," << ky[i] << ") = " << kc[i] << "\n";
    }

    // Comprobar isWall e isGoal
    cout << "Celda (0,0) es pared? " << board.isWall(0,0) << "\n";
    cout << "Celda (3,2) es goal? " << board.isGoal(3,2) << "\n";
    assert(board.isWall(0,0));
    assert(board.isGoal(3,2));

    // Limpieza
    delete[] bx;
    delete[] by;
    delete[] locked;
    delete[] kx;
    delete[] ky;
    delete[] kc;

    cout << "\nAll Board tests passed!\n";
    return 0;
}
