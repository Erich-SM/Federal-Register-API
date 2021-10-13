# Federal Registry API in C
# See LICENSE for copyright and other such details

SRC = fdr.c jsonparse.c
OBJ = ${SRC:.c=.o}

LDFLAGS=-lcurl -lcjson
CFLAGS=-std=c99 -pedantic -Wall -Wno-deprecated-delarations

all: fdr

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}:

fdr: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f fdr fdr-${VERSION}.tar.gz *.o
