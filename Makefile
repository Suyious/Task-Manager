app: task.o
	g++ task.o -o task.out

task.o: task.cpp
	g++ -c -std=c++11 -Wall task.cpp -o task.o

clean:
	rm -f task.o task.out
