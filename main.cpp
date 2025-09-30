#include "SokobanSolver.h"
#include "Board.h"
#include "State.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main() {
    cout << "TESTER DE SOKOBAN" << endl;
    cout << "====================" << endl;

    bool continuar = true;
    while (continuar) {
        char filename[200];

        // Pedir archivo por consola
        cout << "\nIngrese el nombre del archivo de prueba (testBoards/<nombreArchivo>): ";
        cin.getline(filename, sizeof(filename));

        cout << "\n=== PRUEBA CON ARCHIVO " << filename << " ===" << endl;

        // Verificar que el archivo existe
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: No se pudo abrir " << filename << endl;
            cerr << "Asegúrese de que el archivo está en el directorio correcto" << endl;
        } else {
            file.close();

            // Cargar tablero
            Board board(filename);
            board.printInfo();
            cout << "\nTablero visualizado:" << endl;
            board.printBoard();

            // Posición inicial jugador
            int playerX, playerY;
            board.findPlayerStart(playerX, playerY);
            cout << "\nJugador inicial: (" << playerX << ", " << playerY << ")" << endl;

            // Cajas iniciales
            int *boxX, *boxY;
            char *lockedBoxesChar;
            int numBoxes;
            board.findBoxesStart(boxX, boxY, lockedBoxesChar, numBoxes);
            cout << "Número de cajas: " << numBoxes << endl;

            // Llaves iniciales
            int *keyX, *keyY;
            char *keyChar;
            int numKeys;
            board.findKeysStart(keyX, keyY, keyChar, numKeys);
            cout << "Número de llaves: " << numKeys << endl;

            // Calcular cajas que faltan ubicar
            int boxesLeft = 0;
            for (int i = 0; i < numBoxes; ++i) {
                bool onGoal = false;
                for (int g = 0; g < board.numGoals; ++g) {
                    if (boxX[i] == board.goalX[g] && boxY[i] == board.goalY[g]) {
                        onGoal = true;
                        break;
                    }
                }
                if (!onGoal) boxesLeft++;
            }

            // Crear estado inicial
            State* initialState = new State(playerX, playerY,
                                            boxX, boxY, lockedBoxesChar, numBoxes, boxesLeft,
                                            keyX, keyY, keyChar, numKeys,
                                            board.energyLimit);

            cout << "\nEstado inicial creado:" << endl;
            cout << "- boxesLeft: " << initialState->boxesLeft << endl;
            cout << "- energia: " << initialState->energia << endl;

            // Crear solver
            SokobanSolver solver(initialState, &board);

            // Resolver
            cout << "\n=== INICIANDO RESOLUCIÓN ===" << endl;
            bool solved = solver.solve();

            if (solved) {
                cout << "¡Solución encontrada!" << endl;
            } else {
                cout << "No se encontró solución" << endl;
            }

            // Limpiar memoria
            delete[] boxX; delete[] boxY; delete[] lockedBoxesChar;
            delete[] keyX; delete[] keyY; delete[] keyChar;
        }

        // Preguntar si seguir
        cout << "\n¿Desea probar otro archivo? (s/n): ";
        string opcion;
        getline(cin, opcion);

        if (opcion.empty() || opcion[0] == 'n' || opcion[0] == 'N') {
            continuar = false;
        }
    }

    cout << "\nEjecución finalizada." << endl;
    return 0;
}
