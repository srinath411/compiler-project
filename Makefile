runCode: driver.c lexer.c parser.c keywords.c tokens.c scanner.c hash_table.c grammar.c parse_table.c parse_tree.c stack.c
	gcc -o output.exe driver.c lexer.c parser.c keywords.c tokens.c scanner.c hash_table.c grammar.c parse_table.c parse_tree.c stack.c
	./output.exe
