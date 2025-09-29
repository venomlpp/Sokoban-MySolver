#include "Board.h"

Board::Board(const char* filename) 
    : width(0), height(0), energyLimit(0), moveCost(1), pushCost(2), 
      grid(nullptr), numGoals(0), goalX(nullptr), goalY(nullptr), numBoxes(0) {
    loadFromFile(filename);
}

Board::~Board() {
    // Liberar matriz del grid
    if (grid != nullptr) {
        for (int i = 0; i < height; i++) {
            delete[] grid[i];
        }
        delete[] grid;
    }
    
    // Liberar arrays de objetivos
    delete[] goalX;
    delete[] goalY;
}

void Board::loadFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    string line;
    string currentSection = "";
    currentRow = 0;

    while (getline(file, line)) {
        // Ignorar líneas vacías
        if (line.empty()) continue;

        // Verificar secciones
        if (line[0] == '[') {
            currentSection = line;
            continue;
        }

        if (currentSection == "[META]") {
            parseMetaSection(line);
        } else if (currentSection == "[BOARD]") {
            parseBoardSection(line);
        }
    }

    file.close();
    
    // Contar objetivos y cajas después de cargar el tablero
    countGoalsAndBoxes();
}

void Board::parseMetaSection(string line) {
    stringstream ss(line);
    string key;
    string value;
    
    if (getline(ss, key, '=') && getline(ss, value)) {
        // Eliminar espacios en blanco
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (key == "WIDTH") {
            width = stoi(value);
        } else if (key == "HEIGHT") {
            height = stoi(value);
        } else if (key == "ENERGY_LIMIT") {
            energyLimit = stoi(value);  // CORRECCIÓN: Ahora sí procesa ENERGY_LIMIT
        } else if (key == "MOVE_COST") {
            moveCost = stoi(value);
        } else if (key == "PUSH_COST") {
            pushCost = stoi(value);
        } else if (key == "NAME") {
            // Podemos almacenar el nombre si queremos, pero no tenemos variable en Board
            // name = value;
            // Por ahora, lo ignoramos porque la clase Board no tiene un campo para el nombre.
        }
        // NOTA: El comentario original decía que NAME y ENERGY_LIMIT se ignoraban,
        // pero ENERGY_LIMIT SÍ debe procesarse porque es esencial para el juego.
    }
}

void Board::parseBoardSection(string line) {
    // Inicializar grid si es necesario
    if (grid == nullptr) {
        grid = new char*[height];
        for (int i = 0; i < height; i++) {
            grid[i] = new char[width];
            for (int j = 0; j < width; j++) grid[i][j] = ' ';
        }
    }

    if (currentRow < height) {
        for (int j = 0; j < width; j++) {
            if (j < (int)line.length()) grid[currentRow][j] = line[j];
            else grid[currentRow][j] = ' ';
        }
        currentRow++;
    }
}


void Board::countGoalsAndBoxes() {
    // Primera pasada: contar objetivos y cajas
    numGoals = 0;
    numBoxes = 0;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == '.') numGoals++;
            else if (grid[i][j] == '$') numBoxes++;
        }
    }
    
    // Crear arrays de objetivos
    goalX = new int[numGoals];
    goalY = new int[numGoals];
    
    int goalIndex = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == '.') {
                goalX[goalIndex] = i;
                goalY[goalIndex] = j;
                goalIndex++;
            }
        }
    }
}

bool Board::isWall(int x, int y) const {
    if (!isValidPosition(x, y)) return true;
    return grid[x][y] == '#';
}

bool Board::isGoal(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return grid[x][y] == '.';
}

bool Board::isValidPosition(int x, int y) const {
    return x >= 0 && x < height && y >= 0 && y < width;
}

char Board::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) return '#';
    return grid[x][y];
}

void Board::printInfo() const {
    cout << "=== TABLERO ===" << endl;
    cout << "Dimensiones: " << width << "x" << height << endl;
    cout << "Energía inicial: " << energyLimit << endl;  // Ahora mostrará el valor correcto
    cout << "Costos - Movimiento: " << moveCost << ", Empuje: " << pushCost << endl;
    cout << "Objetivos: " << numGoals << ", Cajas: " << numBoxes << endl;
}

void Board::printBoard() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

void Board::findPlayerStart(int& x, int& y) const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == '@') {
                x = i;
                y = j;
                return;
            }
        }
    }
    x = y = -1; // No encontrado
}

void Board::findBoxesStart(int*& boxX, int*& boxY, int& count) const {
    count = numBoxes;
    boxX = new int[count];
    boxY = new int[count];
    
    int boxIndex = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == '$') {
                boxX[boxIndex] = i;
                boxY[boxIndex] = j;
                boxIndex++;
            }
        }
    }
}