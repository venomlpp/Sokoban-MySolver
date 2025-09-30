#ifndef BOARD_H
#define BOARD_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

class Board {
public:
    int width;          // Ancho del tablero
    int height;         // Alto del tablero
    int energyLimit;    // Energía inicial del jugador
    int moveCost;       // Costo de movimiento simple
    int pushCost;       // Costo de empujar una caja
    char** grid;        // Matriz del tablero (caracteres)
    int numGoals;       // Número de puntos objetivo
    int* goalX;         // Coordenadas X de los objetivos
    int* goalY;         // Coordenadas Y de los objetivos
    int numBoxes;       // Número total de cajas en el nivel
    int currentRow;
    int numKeysStart;
    int *keyXStart;
    int *keyYStart;
    char *keyCharStart;

    // Constructor que carga desde archivo
    Board(const char* filename);
    // Destructor
    ~Board();

    // Métodos de consulta
    bool isWall(int x, int y) const;
    bool isGoal(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    char getCell(int x, int y) const;
    void printInfo() const;
    void printBoard() const; // Imprime el tablero en consola
    //bool isClosedDoor(int x, int y) const;

    // Métodos para encontrar elementos iniciales
    void findPlayerStart(int& x, int& y) const;
    void findBoxesStart(int*& boxX, int*& boxY, char*& lockedBoxesChar, int& count) const;
    void findKeysStart(int*& keyX, int*& keyY, char*& keyChar, int& count) const;

private:
    void loadFromFile(const char* filename);
    void parseMetaSection(std::string line);
    void parseBoardSection(std::string line);
    void countGoalsAndBoxes();
};

#endif