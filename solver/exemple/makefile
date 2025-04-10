solver_dir = ../
quarto_dir = ../../quarto
heuristic_dir = ../heuristic
holdall_dir = ../holdall
ktree_dir = ../ktree
pqueue_dir = ../pqueue
CC = gcc
CFLAGS = -std=c23 \
	-Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
	-O2 -I$(solver_dir) -I$(quarto_dir) -I$(heuristic_dir) -I$(holdall_dir) \
	-I$(ktree_dir) -I$(pqueue_dir) -DQUARTO_EXT=1
vpath %.c $(solver_dir) $(quarto_dir) $(heuristic_dir) $(holdall_dir) $(ktree_dir) $(pqueue_dir)
vpath %.h $(solver_dir) $(quarto_dir) $(heuristic_dir) $(holdall_dir) $(ktree_dir) $(pqueue_dir)
objects = solver.o heuristic.o quarto.o holdall.o ktree.o pqueue.o main.o
executable = exemple
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable) 

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

quarto.o: quarto.c quarto.h
solver.o: solver.c solver.h quarto.h holdall.h ktree.h pqueue.h
heuristic.o: heuristic.c heuristic.h quarto.h solver.h
holdall.o: holdall.c holdall.h
pqueue.o: pqueue.c pqueue.h
ktree.o: ktree.c ktree.h
main.o: main.c solver.h quarto.h heuristic.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)