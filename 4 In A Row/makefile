all: main

clean:
	-rm main.o main graphics.o games.o data.o abPruning.o rules.o score.o

main: main.o graphics.o games.o data.o abPruning.o rules.o score.o
	gcc  -o main main.o graphics.o games.o data.o abPruning.o rules.o score.o -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

main.o: main.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm main.c `sdl-config --cflags`

graphics.o: graphics.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm graphics.c `sdl-config --cflags`	

games.o: games.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm games.c `sdl-config --cflags`	

data.o: data.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm data.c `sdl-config --cflags`	


abPruning.o: abPruning.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm abPruning.c `sdl-config --cflags`	
rules.o: rules.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm rules.c `sdl-config --cflags`	
score.o: score.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm score.c `sdl-config --cflags`	

