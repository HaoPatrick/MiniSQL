CC = cc -g
LEX = flex
YACC = bison
CFLAGS = -DYYDEBUG=1

PROGRAMS = sql.exe

all:	${PROGRAMS} clean

sql.exe:	sql.tab.o sql.o
	${CC} -L../API -lapi_c_conn -lapi sql.tab.o sql.o -o $@ 
 

sql.tab.c sql.tab.h:	sql.y
	${YACC} -vd sql.y

sql.c:	sql.l
	${LEX} -o $@ $<

sql.o:	sql.c sql.tab.h


clean:
	rm -f sql.tab.c sql.tab.h sql.c sql.tab.o sql.o  sql.output

.SUFFIXES:	.l .y .c

