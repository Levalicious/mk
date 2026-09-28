# mk - Plan 9 mk for Unix (the 9base port), with one addition: MKROOT defaults to /usr/share/mk when the environment
# does not set it (unix.c). Built standalone against the lib9 vendored under lib9/ (9base's, sources unmodified, see
# lib9/LICENSE and NOTICE). The proto mkfiles every project includes (<$MKROOT/$objtype/mkfile, <$MKROOT/proto/mkone)
# live in the mkroot repository; toposort/ is the helper mkone's all-libs recipe orders library directories with.
#
#   make            build mk and toposort
#   make test       run mkfile.test with the fresh mk
#   make install    install both under $(PREFIX)/bin (default /usr/local; PREFIX=$HOME/.local for a user install)

TARG      = mk
OFILES    = arc.o archive.o bufblock.o env.o file.o graph.o job.o lex.o \
            main.o match.o mk.o parse.o recipe.o rc.o rule.o run.o sh.o \
            shell.o shprint.o symtab.o var.o varsub.o word.o unix.o
MANFILE   = mk.1

PREFIX    ?= /usr/local
MANPREFIX = ${PREFIX}/share/man
MKROOT   ?= /usr/share/mk
OBJTYPE  ?= x86_64

CC        = cc
CFLAGS   += -Wall -Wno-missing-braces -Wno-parentheses -Wno-switch -I. -Ilib9 -Ilib9/sec -DPREFIX="\"${PREFIX}\"" -DMKROOT="\"${MKROOT}\"" -DOBJTYPE="\"${OBJTYPE}\"" -fcommon
LDFLAGS  += -static

all: ${TARG} toposort/toposort

lib9/lib9.a:
	${MAKE} -C lib9

${TARG}: lib9/lib9.a ${OFILES}
	${CC} ${LDFLAGS} -o ${TARG} ${OFILES} -Llib9 -l9 -lm

.c.o:
	${CC} ${CFLAGS} -c $*.c -o $*.o

toposort/toposort: toposort/toposort.c
	${CC} -Wall -O2 -o $@ toposort/toposort.c

test: ${TARG}
	MKROOT=${MKROOT} objtype=${OBJTYPE} ./${TARG} -f mkfile.test use-sh

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin ${DESTDIR}${MANPREFIX}/man1
	cp -f ${TARG} toposort/toposort ${DESTDIR}${PREFIX}/bin/
	chmod 755 ${DESTDIR}${PREFIX}/bin/${TARG} ${DESTDIR}${PREFIX}/bin/toposort
	cp -f ${MANFILE} ${DESTDIR}${MANPREFIX}/man1/
	chmod 444 ${DESTDIR}${MANPREFIX}/man1/${MANFILE}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${TARG} ${DESTDIR}${PREFIX}/bin/toposort ${DESTDIR}${MANPREFIX}/man1/${MANFILE}

clean:
	rm -f ${OFILES} ${TARG} toposort/toposort
	${MAKE} -C lib9 clean

.PHONY: all test install uninstall clean
