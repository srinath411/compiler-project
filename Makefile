runCode: driver.c keywords.c
	gcc -c driver.c
	gcc -c keywords.c
	gcc driver.o keywords.o -o output.exe
	./output.exe