# Federal Registry API in C
# See LICENSE for copyright and other such details

SRC = fdr.c 
OBJ = ${SRC:%.c=%.o}

LDFLAGS=-lcurl -lcjson
CFLAGS=-c -fPIC -std=c99 -pedantic -Wall -Wno-deprecated-delarations

all: static

static:
	gcc -c ${SRC} -o ${OBJ}
	ar -rc libfdr.a ${OBJ}

install: static 
	cp libfdr.a /usr/lib/libfdr.a

clean:
	rm -f fdr fdr-${VERSION}.tar.gz *.o *.a
