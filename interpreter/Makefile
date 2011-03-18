CC=gcc
CFLAGS=-Wall -g

objects = apply.o env.o eval.o lib/list.o lib/str_utils.o \
	main.o obarray.o primitives.o reader.o storage.o syntax.o


lispinc : $(objects)
	gcc -Wall -g -o lispinc $(objects)

main.o : apply.h env.h eval.h reader.h storage.h types.h 
apply.o : apply.h env.h eval.h lib/str_utils.h obarray.h \
	primitives.h reader.h storage.h types.h
eval.o: apply.h env.h eval.h storage.h syntax.h types.h
storage.o : env.h obarray.h storage.h types.h
env.o : env.h storage.h types.h
obarray.o : obarray.h
primitives.o : apply.h eval.h primitives.h reader.h storage.h types.h
reader.o : lib/list.h lib/str_utils.h reader.h storage.h
syntax.o : lib/str_utils.h storage.h syntax.h
lib/str_utils.o : lib/str_utils.h
lib/list.o : lib/list.h

.PHONY : clean
clean :
	rm lispinc $(objects)
