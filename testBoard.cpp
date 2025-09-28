#include "/home/v3nomlpp/Escritorio/Code/FreshSokoban/Board.h"
#include <cassert>

using namespace std;

void testBoardLoading() {
    cout << "=== Test: Cargar tablero desde archivo ===" << endl;
    
    // Crear un archivo de prueba temporal
    ofstream testFile("test_level.txt");
    testFile << "[META]" << endl;
    testFile << "WIDTH=7" << endl;
    testFile << "HEIGHT=6" << endl;
    testFile << "ENERGY_LIMIT=100" << endl;
    testFile << "MOVE_COST=1" << endl;
    testFile << "PUSH_COST=2" << endl;
    testFile << "NAME=Test Level" << endl;
    testFile << "[BOARD]" << endl;
    testFile << "#######" << endl;
    testFile << "#@    #" << endl;
    testFile << "#     #" << endl;
    testFile << "#  $  #" << endl;
    testFile << "#  .  #" << endl;
    testFile << "#######" << endl;
    testFile.close();

    // Cargar el tablero
    Board board("test_level.txt");
    
    // Verificar metadatos
    assert(board.width == 7);
    assert(board.height == 6);
    assert(board.energyLimit == 100);
    assert(board.moveCost == 1);
    assert(board.pushCost == 2);
    
    cout << "✓ Metadatos cargados correctamente" << endl;

    // Verificar dimensiones del grid
    assert(board.grid != nullptr);
    assert(board.height == 6);
    assert(board.width == 7);
    
    cout << "✓ Grid dimensionado correctamente" << endl;

    // Verificar contenido específico del grid
    assert(board.getCell(0, 0) == '#');
    assert(board.getCell(1, 1) == '@');
    assert(board.getCell(3, 3) == '$');
    assert(board.getCell(4, 3) == '.');
    
    cout << "✓ Contenido del grid verificado" << endl;

    // Verificar conteo de objetivos y cajas
    assert(board.numGoals == 1);
    assert(board.numBoxes == 1);
    
    cout << "✓ Objetivos y cajas contados correctamente" << endl;

    // Verificar coordenadas de objetivos
    assert(board.goalX[0] == 4);
    assert(board.goalY[0] == 3);
    
    cout << "✓ Coordenadas de objetivos correctas" << endl;

    // Limpiar archivo temporal
    remove("test_level.txt");
    
    cout << "✓ Test de carga completado exitosamente\n" << endl;
}

void testBoardFunctions() {
    cout << "=== Test: Funciones de consulta del tablero ===" << endl;
    
    // Crear un archivo de prueba más complejo
    ofstream testFile("test_level2.txt");
    testFile << "[META]" << endl;
    testFile << "WIDTH=5" << endl;
    testFile << "HEIGHT=5" << endl;
    testFile << "ENERGY_LIMIT=50" << endl;
    testFile << "[BOARD]" << endl;
    testFile << "#####" << endl;
    testFile << "#@$.#" << endl;
    testFile << "#   #" << endl;
    testFile << "#  .#" << endl;
    testFile << "#####" << endl;
    testFile.close();

    Board board("test_level2.txt");
    
    // Test isWall
    assert(board.isWall(0, 0) == true);
    assert(board.isWall(1, 1) == false);
    assert(board.isWall(10, 10) == true); // Fuera de límites
    
    cout << "✓ Función isWall funciona correctamente" << endl;

    // Test isGoal
    assert(board.isGoal(1, 3) == true);
    assert(board.isGoal(3, 3) == true);
    assert(board.isGoal(1, 1) == false);
    
    cout << "✓ Función isGoal funciona correctamente" << endl;

    // Test isValidPosition
    assert(board.isValidPosition(0, 0) == true);
    assert(board.isValidPosition(4, 4) == true);
    assert(board.isValidPosition(-1, 0) == false);
    assert(board.isValidPosition(0, 5) == false);
    
    cout << "✓ Función isValidPosition funciona correctamente" << endl;

    // Test findPlayerStart
    int playerX, playerY;
    board.findPlayerStart(playerX, playerY);
    assert(playerX == 1 && playerY == 1);
    
    cout << "✓ Posición inicial del jugador encontrada correctamente" << endl;

    // Test findBoxesStart
    int *boxX, *boxY;
    int boxCount;
    board.findBoxesStart(boxX, boxY, boxCount);
    assert(boxCount == 1);
    assert(boxX[0] == 1 && boxY[0] == 2);
    delete[] boxX;
    delete[] boxY;
    
    cout << "✓ Posiciones iniciales de cajas encontradas correctamente" << endl;

    // Limpiar archivo temporal
    remove("test_level2.txt");
    
    cout << "✓ Test de funciones completado exitosamente\n" << endl;
}

void testVisualization() {
    cout << "=== Test: Visualización del tablero ===" << endl;
    
    // Crear archivo de prueba simple
    ofstream testFile("test_visual.txt");
    testFile << "[META]" << endl;
    testFile << "WIDTH=3" << endl;
    testFile << "HEIGHT=3" << endl;
    testFile << "[BOARD]" << endl;
    testFile << "###" << endl;
    testFile << "#@#" << endl;
    testFile << "###" << endl;
    testFile.close();

    Board board("test_visual.txt");
    
    cout << "Tablero cargado:" << endl;
    board.printBoard();
    
    cout << "\nInformación del tablero:" << endl;
    board.printInfo();
    
    // Limpiar archivo temporal
    remove("test_visual.txt");
    
    cout << "✓ Test de visualización completado\n" << endl;
}

int main() {
    cout << "Iniciando pruebas de la clase Board...\n" << endl;
    
    testBoardLoading();
    testBoardFunctions();
    testVisualization();
    
    cout << "¡Todas las pruebas pasaron exitosamente!" << endl;
    return 0;
}