#include "../HashTable.h"
#include <cassert>
#include <iostream>
using namespace std;

int main() {
    cout << "RUNNING HashTable basic tests...\n";

    // Crear algunos estados simples
    int bx1[] = {1};
    int by1[] = {2};
    char locked1[] = {'\0'};
    State s1(0, 0, bx1, by1, locked1, 1, 1, nullptr, nullptr, nullptr, 0, 50);

    int bx2[] = {2};
    int by2[] = {3};
    char locked2[] = {'\0'};
    State s2(1, 1, bx2, by2, locked2, 1, 1, nullptr, nullptr, nullptr, 0, 50);

    int bx3[] = {4};
    int by3[] = {5};
    char locked3[] = {'\0'};
    State s3(2, 2, bx3, by3, locked3, 1, 1, nullptr, nullptr, nullptr, 0, 50);

    // Crear tabla con baja capacidad para forzar rehash rápido
    HashTable table(2, 0.5);

    // Insertar estados
    table.insert(&s1);
    assert(table.contains(&s1) && "Tabla debe contener s1");

    table.insert(&s2);
    assert(table.contains(&s2) && "Tabla debe contener s2");

    table.insert(&s3); // aquí debería rehashear
    assert(table.contains(&s3) && "Tabla debe contener s3");

    cout << "Todos los estados insertados correctamente.\n";

    // Mostrar estadísticas
    table.printStats();
    table.printDetailedStats();

    // Limpiar
    table.clear();
    assert(!table.contains(&s1) && "Después de clear, no debe contener s1");

    cout << "\nAll HashTable tests passed!\n";
    return 0;
}