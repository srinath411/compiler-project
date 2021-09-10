runCode: driver.c common/error_handler.c lexer/lexer.c parser/parser.c lexer/keywords.c common/symbols.c lexer/token_stream.c utils/scanner.c utils/hash_table.c parser/grammar.c parser/parse_table.c parser/parse_tree.c utils/stack.c utils/bit_array.c
	gcc -o output.exe driver.c common/error_handler.c lexer/lexer.c parser/parser.c lexer/keywords.c common/symbols.c lexer/token_stream.c utils/scanner.c utils/hash_table.c parser/grammar.c parser/parse_table.c parser/parse_tree.c utils/stack.c utils/bit_array.c
	./output.exe
