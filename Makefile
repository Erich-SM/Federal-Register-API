# Federal Registry API in C
# See LICENSE for copyright and other such details

SRC = fdr.c 
OBJ = ${SRC:%.c=%.o}

LDFLAGS=-lcurl -lcjson
CFLAGS=-c -fPIC -std=c99 -pedantic -Wall -Wno-deprecated-delarations

all: pick

pick:
	@echo "pick either static or dynamic"
	@echo "make static"
	@echo "make dynamic"
	@echo "make install-static"
	@echo "make install-dynamic"

static:
	gcc  -c ${SRC} -o ${OBJ}
	ar -rc libfdr.a ${OBJ}

dynamic:
	gcc -Wall -fPIC -c *.c
	gcc -shared -Wl,-soname,libfdr.so -o libfdr.so *.o


install-static: static 
	cp libfdr.a /usr/lib/libfdr.a

install-dynamic: dynamic
	cp libfdr.so /usr/lib/libfdr.so

clean:
	rm -f fdr fdr-${VERSION}.tar.gz *.o *.a
