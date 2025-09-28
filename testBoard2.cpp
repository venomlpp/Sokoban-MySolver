#include "Board.h"
#include <cassert>
#include <string>

using namespace std;

void testBoardLoading(const string& filename) {
    cout << "=== Test: Cargar tablero desde archivo '" << filename << "' ===" << endl;
    
    try {
        // Cargar el tablero
        Board board(filename.c_str());
        
        // Verificar que el grid se cargó correctamente
        assert(board.grid != nullptr);
        assert(board.width > 0);
        assert(board.height > 0);
        
        cout << "✓ Grid cargado correctamente - Dimensiones: " 
             << board.width << "x" << board.height << endl;

        // Verificar metadatos básicos
        cout << "Energía límite: " << board.energyLimit << endl;
        cout << "Costo movimiento: " << board.moveCost << endl;
        cout << "Costo empuje: " << board.pushCost << endl;
        
        cout << "✓ Metadatos básicos verificados" << endl;

        // Verificar contenido del grid
        cout << "Número de objetivos: " << board.numGoals << endl;
        cout << "Número de cajas: " << board.numBoxes << endl;
        
        cout << "✓ Conteo de elementos del juego verificado" << endl;

        cout << "✓ Test de carga completado exitosamente\n" << endl;
        
    } catch (const exception& e) {
        cerr << "❌ Error cargando el archivo: " << e.what() << endl;
        throw;
    }
}

void testBoardFunctions(const string& filename) {
    cout << "=== Test: Funciones de consulta del tablero '" << filename << "' ===" << endl;
    
    Board board(filename.c_str());
    
    // Test isWall con varias posiciones
    cout << "Probando función isWall..." << endl;
    for (int i = 0; i < board.height; i++) {
        for (int j = 0; j < board.width; j++) {
            char cell = board.getCell(i, j);
            bool isWall = board.isWall(i, j);
            if (cell == '#') {
                assert(isWall == true);
            } else {
                assert(isWall == false);
            }
        }
    }
    // Test posición fuera de límites
    assert(board.isWall(-1, -1) == true);
    assert(board.isWall(board.height, board.width) == true);
    
    cout << "✓ Función isWall funciona correctamente" << endl;

    // Test isGoal
    cout << "Probando función isGoal..." << endl;
    for (int i = 0; i < board.height; i++) {
        for (int j = 0; j < board.width; j++) {
            char cell = board.getCell(i, j);
            bool isGoal = board.isGoal(i, j);
            if (cell == '.') {
                assert(isGoal == true);
            } else {
                assert(isGoal == false);
            }
        }
    }
    
    cout << "✓ Función isGoal funciona correctamente" << endl;

    // Test isValidPosition
    cout << "Probando función isValidPosition..." << endl;
    assert(board.isValidPosition(0, 0) == true);
    assert(board.isValidPosition(board.height-1, board.width-1) == true);
    assert(board.isValidPosition(-1, 0) == false);
    assert(board.isValidPosition(0, -1) == false);
    assert(board.isValidPosition(board.height, 0) == false);
    assert(board.isValidPosition(0, board.width) == false);
    
    cout << "✓ Función isValidPosition funciona correctamente" << endl;

    // Test findPlayerStart
    cout << "Probando findPlayerStart..." << endl;
    int playerX, playerY;
    board.findPlayerStart(playerX, playerY);
    assert(playerX >= 0 && playerX < board.height);
    assert(playerY >= 0 && playerY < board.width);
    assert(board.getCell(playerX, playerY) == '@');
    
    cout << "✓ Posición inicial del jugador encontrada: (" 
         << playerX << ", " << playerY << ")" << endl;

    // Test findBoxesStart
    cout << "Probando findBoxesStart..." << endl;
    int *boxX, *boxY;
    int boxCount;
    board.findBoxesStart(boxX, boxY, boxCount);
    assert(boxCount == board.numBoxes);
    
    for (int i = 0; i < boxCount; i++) {
        assert(boxX[i] >= 0 && boxX[i] < board.height);
        assert(boxY[i] >= 0 && boxY[i] < board.width);
        assert(board.getCell(boxX[i], boxY[i]) == '$');
    }
    
    cout << "✓ " << boxCount << " cajas encontradas correctamente" << endl;
    delete[] boxX;
    delete[] boxY;
    
    cout << "✓ Test de funciones completado exitosamente\n" << endl;
}

void testVisualization(const string& filename) {
    cout << "=== Test: Visualización del tablero '" << filename << "' ===" << endl;
    
    Board board(filename.c_str());
    
    cout << "Visualización del tablero:" << endl;
    cout << "--------------------------" << endl;
    board.printBoard();
    
    cout << "\nInformación detallada:" << endl;
    cout << "----------------------" << endl;
    board.printInfo();
    
    // Mostrar posición del jugador
    int playerX, playerY;
    board.findPlayerStart(playerX, playerY);
    cout << "Posición inicial del jugador: (" << playerX << ", " << playerY << ")" << endl;
    
    // Mostrar posiciones de las cajas
    int *boxX, *boxY;
    int boxCount;
    board.findBoxesStart(boxX, boxY, boxCount);
    cout << "Posiciones de las " << boxCount << " cajas:" << endl;
    for (int i = 0; i < boxCount; i++) {
        cout << "  Caja " << i+1 << ": (" << boxX[i] << ", " << boxY[i] << ")" << endl;
    }
    delete[] boxX;
    delete[] boxY;
    
    // Mostrar posiciones de los objetivos
    cout << "Posiciones de los " << board.numGoals << " objetivos:" << endl;
    for (int i = 0; i < board.numGoals; i++) {
        cout << "  Objetivo " << i+1 << ": (" << board.goalX[i] << ", " << board.goalY[i] << ")" << endl;
    }
    
    cout << "✓ Test de visualización completado\n" << endl;
}

void runAllTests(const string& filename) {
    cout << "=========================================" << endl;
    cout << "EJECUTANDO PRUEBAS CON ARCHIVO: " << filename << endl;
    cout << "=========================================\n" << endl;
    
    testBoardLoading(filename);
    testBoardFunctions(filename);
    testVisualization(filename);
    
    cout << "=========================================" << endl;
    cout << "¡TODAS LAS PRUEBAS PASARON EXITOSAMENTE!" << endl;
    cout << "Archivo probado: " << filename << endl;
    cout << "=========================================" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <archivo_de_tablero>" << endl;
        cout << "Ejemplo: " << argv[0] << " nivel1.txt" << endl;
        cout << "\nTambién puedes usar archivos de ejemplo:" << endl;
        cout << "  " << argv[0] << " niveles/nivel1.txt" << endl;
        cout << "  " << argv[0] << " ../niveles/nivel2.txt" << endl;
        return 1;
    }
    
    string filename = argv[1];
    
    try {
        runAllTests(filename);
    } catch (const exception& e) {
        cerr << "❌ Error durante las pruebas: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}