main: main.o
	g++ $< -o $@


main.o: main.cc
	g++ main.cc -c -O2 

profile: main.cc
	g++ main.cc -O2 -pg -g
 
clean:
	rm *.o main *.out