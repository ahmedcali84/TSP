build:
	gcc -Wall -Werror -Wextra -ggdb -o build main.c -lm
clean:
	rm -rf build