# Makefile para compilar el proyecto SokobanSolver
# Autor: Luis Peluk Pino
all: main testState testHeap testOperation testBoard testHashTable

# Ejecutable principal (no por ahora)
main: main.cpp SokobanSolver.o Board.o State.o Heap.o Operation.o HashTable.o
	g++ main.cpp SokobanSolver.o Board.o State.o Heap.o Operation.o HashTable.o -o main


# Test simple para la clase State (fuente en tests/testState.cpp)
testState: tests/testState.cpp State.o
	g++ tests/testState.cpp State.o -o testState

testHeap: tests/testHeap.cpp Heap.o State.o
	g++ tests/testHeap.cpp Heap.o State.o -o testHeap

testOperation: tests/testOperation.cpp Operation.o State.o Board.o
	g++ tests/testOperation.cpp Operation.o State.o Board.o -o testOperation

testBoard: tests/testBoard.cpp Board.o
	g++ tests/testBoard.cpp Board.o -o testBoard

testHashTable: tests/testHashTable.cpp HashTable.o State.o
	g++ tests/testHashTable.cpp HashTable.o State.o -o testHashTable

# Objetos
State.o: State.cpp State.h
	$(CC) $(CFLAGS) -c State.cpp -o State.o

Board.o: Board.cpp Board.h
	$(CC) $(CFLAGS) -c Board.cpp -o Board.o

Heap.o: Heap.cpp Heap.h
	$(CC) $(CFLAGS) -c Heap.cpp -o Heap.o

HashTable.o: HashTable.cpp HashTable.h
	$(CC) $(CFLAGS) -c HashTable.cpp -o HashTable.o

Operation.o: Operation.cpp Operation.h State.h Board.h
	$(CC) $(CFLAGS) -c Operation.cpp -o Operation.o

SokobanSolver.o: SokobanSolver.cpp SokobanSolver.h State.h Board.h Operation.h Heap.h HashTable.h
	$(CC) $(CFLAGS) -c SokobanSolver.cpp -o SokobanSolver.o

# Target conveniente para ejecutar el testState con un tablero de testBoards/
runTestState: testState
	./testState testBoards/Mediano-1.txt

clean:
	rm -f *.o main testState testHeap testOperation testBoard testHashTable