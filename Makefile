# Всі об'єктні файли, які потрібні для проєкту
OBJS = Polygone.o Predicat.o vectors.o linerays.o testsPolygone.o main.o file_forming.o

# Ім'я фінального виконуваного файлу
TARGET = RunTests.exe

# Команда, яка виконується за замовчуванням (коли ви просто пишете 'make')
all: $(TARGET)

# Правило для створення фінального файлу з об'єктних
$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lm

# Правила для компіляції кожного .c файлу в .o файл
Polygone.o: Polygone.c
	gcc -c Polygone.c -o Polygone.o

Predicat.o: Predicat.c
	gcc -c Predicat.c -o Predicat.o

vectors.o: vectors.c
	gcc -c vectors.c -o vectors.o

linerays.o: linerays.c
	gcc -c linerays.c -o linerays.o

testsPolygone.o: testsPolygone.c
	gcc -c testsPolygone.c -o testsPolygone.o

main.o: main.c
	gcc -c main.c -o main.o

file_forming.o: file_forming.c
	gcc -c file_forming.c -o file_forming.o

clean:
	del *.o $(TARGET)