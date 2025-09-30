#ifndef BOARD_H
#define BOARD_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

class Board {
public:
    int width;          
    int height;
    int energyLimit;
    int moveCost;
    int pushCost;
    char** grid;
    int numGoals;
    int* goalX;
    int* goalY;
    int numBoxes;
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