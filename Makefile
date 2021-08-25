runCode: driver.c lexer.c keywords.c tokens.c scanner.c
	gcc -o output.exe driver.c lexer.c keywords.c tokens.c scanner.c
	./output.exe
