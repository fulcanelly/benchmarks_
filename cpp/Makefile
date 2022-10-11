CC = clang++

main: main.o
	$(CC) $< -o $@



main.o: main.cc
	$(CC) main.cc -c -O2 -pthread

profile: main.cc
	$(CC) main.cc -O2 -pg -g -fsanitize=address   
 
clean:
	rm *.o main *.out