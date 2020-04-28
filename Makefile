GCC = gcc -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic
VAL = valgrind --tool=memcheck --log-file=memcheckm --leak-check=full --verbose --track-origins=yes

SRCS = main.c code.c

CodeGeneration: $(SRCS)
	$(GCC) $(SRCS) -o CodeGeneration

	
testmemory: CodeGeneration
	$(VAL) ./CodeGeneration testcases/combined combined
	
testall: basic repeat overflow alternate_tail combined
	
basic: CodeGeneration
	./CodeGeneration testcases/basic basic
	
repeat: CodeGeneration
	./CodeGeneration testcases/repeat repeat

overflow: CodeGeneration
	-./CodeGeneration testcases/overflow overflow

alternate_tail: CodeGeneration
	./CodeGeneration testcases/tail tail

combined: CodeGeneration
	./CodeGeneration testcases/combined combined

remove:
	-rm basic
	-rm repeat
	-rm tail
	-rm combined