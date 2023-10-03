asm: main.o 
	g++ -o assembler main.o 


main.o: main.cpp 
	g++ -c -std=gnu++2b  main.cpp

clean:
	rm *.o assembler