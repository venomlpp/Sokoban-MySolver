#include "SokobanSolver.h"
#include "Board.h"
#include "State.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void testWithFile(const char* filename) {
    cout << "=== PRUEBA CON ARCHIVO " << filename << " ===" << endl;
    
    // Verificar que el archivo existe
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir " << filename << endl;
        cerr << "Asegúrate de que el archivo está en el mismo directorio" << endl;
        return;
    }
    file.close();
    
    // Cargar el tablero
    Board board(filename);
    board.printInfo();
    cout << "\nTablero visualizado:" << endl;
    board.printBoard();
    
    // Encontrar posición inicial del jugador
    int playerX, playerY;
    board.findPlayerStart(playerX, playerY);
    cout << "\nJugador inicial: (" << playerX << ", " << playerY << ")" << endl;
    
    // Encontrar cajas iniciales
    int *boxX, *boxY;
    char *lockedBoxesChar;
    int numBoxes;
    board.findBoxesStart(boxX, boxY, lockedBoxesChar, numBoxes);
    cout << "Número de cajas: " << numBoxes << endl;
    for (int i = 0; i < numBoxes; i++) {
        cout << "Caja " << i << ": (" << boxX[i] << ", " << boxY[i] << ")" << endl;
        if (lockedBoxesChar[i] != '\0') {
            cout << "  - Bloqueada con: " << lockedBoxesChar[i] << endl;
        }
    }

    // llaves iniciales
    int *keyX, *keyY;
    char *keyChar;
    int numKeys;
    board.findKeysStart(keyX, keyY, keyChar, numKeys);

// Mostrar llaves
    cout << "Número de llaves: " << numKeys << endl;
    for (int i = 0; i < numKeys; i++) {
        cout << "Llave " << i << ": (" << keyX[i] << ", " << keyY[i] << ") - " << keyChar[i] << endl;
    }

    // Mostrar objetivos
    cout << "Número de objetivos: " << board.numGoals << endl;
    for (int i = 0; i < board.numGoals; i++) {
        cout << "Objetivo " << i << ": (" << board.goalX[i] << ", " << board.goalY[i] << ")" << endl;
    }

    int boxesLeft = 0;
    for (int i = 0; i < numBoxes; ++i) {
        bool onGoal = false;
        for (int g = 0; g < board.numGoals; ++g)
            if (boxX[i] == board.goalX[g] && boxY[i] == board.goalY[g]) { onGoal = true; break; }
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
    
    // Intentar resolver
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

int main(int argc, char* argv[]) {
    cout << "TESTER DE SOKOBAN" << endl;
    cout << "====================" << endl;
    
    try {
        if (argc > 1) {
            // Usar archivo pasado como argumento
            testWithFile(argv[1]);
        } else {
            // Pedir nombre de archivo al usuario
            char filename[100];
            cout << "Ingrese el nombre del archivo de prueba: ";
            cin.getline(filename, sizeof(filename));
            testWithFile(filename);
        }
        cout << "\nTEST FINALIZADO" << endl;
    } catch (const exception& e) {
        cerr << "Error durante el test: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}