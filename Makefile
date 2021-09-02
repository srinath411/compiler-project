runCode: driver.c lexer/lexer.c parser/parser.c lexer/keywords.c common/symbols.c lexer/token_stream.c utils/scanner.c utils/hash_table.c parser/grammar.c parser/parse_table.c parser/parse_tree.c utils/stack.c
	gcc -o output.exe driver.c lexer/lexer.c parser/parser.c lexer/keywords.c common/symbols.c lexer/token_stream.c utils/scanner.c utils/hash_table.c parser/grammar.c parser/parse_table.c parser/parse_tree.c utils/stack.c
	./output.exe
