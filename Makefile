OUTPUT=run
build:
	gcc -o3 -Wall -Werror -Wextra -std=c17 -ggdb -pedantic-errors -o $(OUTPUT) main.c -lm
clean:
	$(RM) -r *.dSYM *.o *.txt *.bin $(OUTPUT)