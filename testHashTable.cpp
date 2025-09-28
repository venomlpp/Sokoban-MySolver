#include "HashTable.h"
#include "State.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

void testConstructor() {
    cout << "=== Test: Constructores ===" << endl;
    
    // Test constructor por defecto
    HashTable table1;
    assert(table1.getSize() == 0);
    assert(table1.getCapacity() == 1000);
    assert(table1.getLoadFactor() == 0.0);
    cout << "✓ Constructor por defecto funciona correctamente" << endl;
    
    // Test constructor con parámetros
    HashTable table2(500, 0.5);
    assert(table2.getSize() == 0);
    assert(table2.getCapacity() == 500);
    assert(table2.getLoadFactorThreshold() == 0.5);
    cout << "✓ Constructor con parámetros funciona correctamente" << endl;
    
    cout << "✓ Test de constructores completado exitosamente\n" << endl;
}

void testBasicInsertAndContains() {
    cout << "=== Test: Inserción y búsqueda básica ===" << endl;
    
    HashTable table(10, 0.7); // Tabla pequeña para forzar colisiones
    
    // Crear estados de prueba
    int boxX1[] = {1, 2}; int boxY1[] = {1, 2};
    State* state1 = new State(0, 0, boxX1, boxY1, 2, 100);
    
    int boxX2[] = {3, 4}; int boxY2[] = {3, 4};
    State* state2 = new State(1, 1, boxX2, boxY2, 2, 90);
    
    int boxX3[] = {5, 6}; int boxY3[] = {5, 6};
    State* state3 = new State(2, 2, boxX3, boxY3, 2, 80);
    
    // Verificar que la tabla está vacía
    assert(table.contains(state1) == false);
    assert(table.contains(state2) == false);
    assert(table.contains(state3) == false);
    assert(table.getSize() == 0);
    cout << "✓ Tabla vacía identificada correctamente" << endl;
    
    // Insertar primer estado
    table.insert(state1);
    assert(table.contains(state1) == true);
    assert(table.getSize() == 1);
    cout << "✓ Primer estado insertado y encontrado correctamente" << endl;
    
    // Insertar segundo estado
    table.insert(state2);
    assert(table.contains(state2) == true);
    assert(table.getSize() == 2);
    cout << "✓ Segundo estado insertado y encontrado correctamente" << endl;
    
    // Insertar tercer estado
    table.insert(state3);
    assert(table.contains(state3) == true);
    assert(table.getSize() == 3);
    cout << "✓ Tercer estado insertado y encontrado correctamente" << endl;
    
    // Verificar que todos los estados siguen presentes
    assert(table.contains(state1) == true);
    assert(table.contains(state2) == true);
    assert(table.contains(state3) == true);
    cout << "✓ Todos los estados permanecen en la tabla después de inserciones múltiples" << endl;
    
    cout << "✓ Test de inserción y búsqueda básica completado exitosamente\n" << endl;
}

void testStateEquality() {
    cout << "=== Test: Igualdad de estados ===" << endl;
    
    HashTable table;
    
    // Crear estados idénticos
    int boxX1[] = {1, 2}; int boxY1[] = {1, 2};
    State* state1 = new State(0, 0, boxX1, boxY1, 2, 100);
    
    int boxX2[] = {1, 2}; int boxY2[] = {1, 2};
    State* state2 = new State(0, 0, boxX2, boxY2, 2, 100);
    
    // Crear estado diferente
    int boxX3[] = {3, 4}; int boxY3[] = {3, 4};
    State* state3 = new State(1, 1, boxX3, boxY3, 2, 100);
    
    // Insertar primer estado
    table.insert(state1);
    
    // Verificar que el estado idéntico es detectado como contenido
    assert(table.contains(state2) == true);
    cout << "✓ Estados idénticos son correctamente identificados como iguales" << endl;
    
    // Verificar que estado diferente no está en la tabla
    assert(table.contains(state3) == false);
    cout << "✓ Estado diferente es correctamente identificado como no presente" << endl;
    
    cout << "✓ Test de igualdad de estados completado exitosamente\n" << endl;
}

void testCollisionsAndRehashing() {
    cout << "=== Test: Colisiones y rehashing ===" << endl;
    
    HashTable table(5, 0.6); // Tabla muy pequeña para forzar colisiones y rehashing
    
    cout << "Capacidad inicial: " << table.getCapacity() << endl;
    cout << "Umbral de factor de carga: " << table.getLoadFactorThreshold() << endl;
    
    // Crear múltiples estados para forzar colisiones
    vector<State*> states;
    for (int i = 0; i < 10; i++) {
        int boxX[] = {i}; int boxY[] = {i};
        State* state = new State(i, i, boxX, boxY, 1, 100 - i);
        states.push_back(state);
        
        table.insert(state);
        
        cout << "Insertado estado " << i << " - Size: " << table.getSize() 
             << ", Capacidad: " << table.getCapacity() 
             << ", Factor de carga: " << table.getLoadFactor() << endl;
        
        // Verificar que el estado insertado está en la tabla
        assert(table.contains(state) == true);
    }
    
    // Verificar que se produjeron colisiones
    assert(table.getCollisionCount() > 0);
    cout << "✓ Se produjeron " << table.getCollisionCount() << " colisiones - CORRECTO" << endl;
    
    // Verificar que se realizó rehashing
    assert(table.getRehashCount() > 0);
    cout << "✓ Se realizó rehashing " << table.getRehashCount() << " veces - CORRECTO" << endl;
    
    // Verificar que todos los estados siguen accesibles después del rehashing
    for (State* state : states) {
        assert(table.contains(state) == true);
    }
    cout << "✓ Todos los estados accesibles después del rehashing - CORRECTO" << endl;
    
    cout << "✓ Test de colisiones y rehashing completado exitosamente\n" << endl;
}

void testEdgeCases() {
    cout << "=== Test: Casos límite ===" << endl;
    
    HashTable table;
    
    // Test con estado nulo (debería manejarse gracefuly)
    // Nota: La implementación actual no maneja nullptr, así que lo omitimos
    
    // Test con estados muy similares (que podrían tener el mismo hash)
    int boxX1[] = {1, 2}; int boxY1[] = {1, 2};
    State* state1 = new State(0, 0, boxX1, boxY1, 2, 100);
    
    int boxX2[] = {2, 1}; int boxY2[] = {2, 1}; // Mismos valores, orden diferente
    State* state2 = new State(0, 0, boxX2, boxY2, 2, 100);
    
    int boxX3[] = {1, 2}; int boxY3[] = {1, 2};
    State* state3 = new State(1, 0, boxX3, boxY3, 2, 100); // Solo X diferente
    
    table.insert(state1);
    
    // Estados con mismos valores pero orden diferente deberían ser diferentes
    assert(table.contains(state2) == false);
    cout << "✓ Estados con cajas en orden diferente son correctamente distinguidos" << endl;
    
    // Estado con posición de jugador diferente
    assert(table.contains(state3) == false);
    cout << "✓ Estados con posición de jugador diferente son correctamente distinguidos" << endl;
    
    // Test de clear
    table.clear();
    assert(table.getSize() == 0);
    assert(table.contains(state1) == false);
    cout << "✓ Clear funciona correctamente - tabla vaciada" << endl;
    
    cout << "✓ Test de casos límite completado exitosamente\n" << endl;
}

void testStatistics() {
    cout << "=== Test: Estadísticas ===" << endl;
    
    HashTable table(10, 0.7);
    
    // Insertar algunos estados
    for (int i = 0; i < 5; i++) {
        int boxX[] = {i}; int boxY[] = {i};
        State* state = new State(i, i, boxX, boxY, 1, 100);
        table.insert(state);
    }
    
    // Verificar que las estadísticas son consistentes
    assert(table.getSize() == 5);
    assert(table.getTotalInsertions() == 5);
    assert(table.getLoadFactor() >= 0.0 && table.getLoadFactor() <= 1.0);
    assert(table.getMaxChainLength() >= 1);
    
    cout << "Estadísticas básicas:" << endl;
    cout << "- Size: " << table.getSize() << endl;
    cout << "- Total insertions: " << table.getTotalInsertions() << endl;
    cout << "- Load factor: " << table.getLoadFactor() << endl;
    cout << "- Max chain length: " << table.getMaxChainLength() << endl;
    cout << "- Collision count: " << table.getCollisionCount() << endl;
    cout << "- Rehash count: " << table.getRehashCount() << endl;
    
    // Probar la función de impresión de estadísticas
    cout << "\nEstadísticas detalladas:" << endl;
    table.printStats();
    
    cout << "✓ Estadísticas calculadas correctamente" << endl;
    cout << "✓ Test de estadísticas completado exitosamente\n" << endl;
}

int main() {
    cout << "=========================================" << endl;
    cout << "INICIANDO PRUEBAS DE LA CLASE HASHTABLE" << endl;
    cout << "=========================================\n" << endl;
    
    try {
        testConstructor();
        testBasicInsertAndContains();
        testStateEquality();
        testCollisionsAndRehashing();
        testEdgeCases();
        testStatistics();
        
        cout << "=========================================" << endl;
        cout << "¡TODAS LAS PRUEBAS PASARON EXITOSAMENTE!" << endl;
        cout << "=========================================" << endl;
        
    } catch (const exception& e) {
        cerr << "❌ Error durante las pruebas: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}   