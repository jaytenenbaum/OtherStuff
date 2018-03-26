all: MacCos MacCos_limit Digits

clean:
	-rm MacCos.o MacCos_limit.o Digits.o MacCos MacCos_limit Digits

MacCos: MacCos.o
	gcc  -o MacCos MacCos.o -lm -std=c99 -pedantic-errors -g

MacCos.o: MacCos.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm MacCos.c
	
MacCos_limit: MacCos_limit.o
	gcc  -o MacCos_limit MacCos_limit.o -lm -std=c99 -pedantic-errors -g

MacCos_limit.o: MacCos_limit.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm MacCos_limit.c

Digits: Digits.o
	gcc -o Digits Digits.o -lm -std=c99 -pedantic-errors -g

Digits.o: Digits.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Digits.c
