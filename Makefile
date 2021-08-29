runCode: driver.c lexer.c keywords.c tokens.c scanner.c hash_table.c
	gcc -o output.exe driver.c lexer.c keywords.c tokens.c scanner.c hash_table.c
	./output.exe
