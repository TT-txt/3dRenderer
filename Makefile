build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o renderer.out

run:
	./renderer.out

clean:
	rm -rf ./renderer.out
