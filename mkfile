# mk - Plan 9 mk ported to Unix
#
<$MKROOT/$objtype/mkfile

TARG=mk

OFILES=\
	arc.$O\
	archive.$O\
	bufblock.$O\
	env.$O\
	file.$O\
	graph.$O\
	job.$O\
	lex.$O\
	main.$O\
	match.$O\
	mk.$O\
	parse.$O\
	recipe.$O\
	rc.$O\
	rule.$O\
	run.$O\
	sh.$O\
	shell.$O\
	shprint.$O\
	symtab.$O\
	var.$O\
	varsub.$O\
	word.$O\
	unix.$O\

HFILES=\
	mk.h\
	fns.h\

LIB=../lib9/lib9.a

CFLAGS=-Wall -O2 -I. -I../lib9 -fcommon
LDFLAGS=-L../lib9 -l9 -lm

<$MKROOT/proto/mkone
