runCode: driver.c lexer/lexer.c parser/parser.c lexer/keywords.c common/tokens.c utils/scanner.c utils/hash_table.c common/grammar.c parser/parse_table.c parser/parse_tree.c utils/stack.c
	gcc -o output.exe driver.c lexer/lexer.c parser/parser.c lexer/keywords.c common/tokens.c utils/scanner.c utils/hash_table.c common/grammar.c parser/parse_table.c parser/parse_tree.c utils/stack.c
	./output.exe
