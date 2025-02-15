all:
	gcc -Wall -Werror -Wextra -ggdb -o build main.c
clean:
	rm *.exe