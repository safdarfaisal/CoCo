compiler : lexer.o driver.o
	gcc lexer.o driver.o -o compiler.exe
lexer.o : lexer.c
	gcc -c lexer.c -o lexer.o
driver.o: driver.c
	gcc -c driver.c -o driver.o
