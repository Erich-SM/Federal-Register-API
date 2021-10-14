# Federal Registry API in C
# See LICENSE for copyright and other such details

SRC = fdr.c 
OBJ = ${SRC:%.c=%.o}

LDFLAGS=-lcurl -lcjson
CFLAGS=-std=c99 -pedantic -Wall -Wno-deprecated-delarations

all: fdr

.c.o:
	${CC} -c ${CFLAGS} $<


fdr: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f fdr fdr-${VERSION}.tar.gz *.o
