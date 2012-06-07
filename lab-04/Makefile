NAME=lab04

INCLUDE=
LIBPATH=
LIBS=-lglut -lGLU -lGL -lm
SYMBOLS=-D_XOPEN_SOURCE

CC=gcc
CFLAGS=-Wall -Werror -std=c99 -pedantic ${SYMBOLS}

LD=gcc
LDFLAGS=${LIBPATH} ${LIBS}

TEX=pdflatex

SRC=${NAME}b.c
OBJ=${SRC:.c=.o}
OUT=${SRC:.c=}

DOCSRC=${NAME}a.tex
DOC=${DOCSRC:.tex=.pdf}

.PHONY: all out doc clean dist

all: out doc

.c.o:
	${CC} ${CFLAGS} -c $<

${OUT}: ${OBJ}
	${LD} ${LDFLAGS} -o $@ ${OBJ}

out: ${OUT}

${DOC}: ${DOCSRC}
	${TEX} ${DOCSRC}

doc: ${DOC}

clean:
	rm -f ${OUT} ${OBJ} ${DOC} ${DOC:.pdf=.aux} ${DOC:.pdf=.log} ${NAME}.tar.bz2

dist: all
	mkdir -p ${NAME}
	cp ${OUT} ${DOC} ${SRC} ${DOCSRC} Makefile ${NAME}
	tar -cjf ${NAME}.tar.bz2 ${NAME}
	rm -rf ${NAME}
