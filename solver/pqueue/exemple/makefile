pqueue_dir = ../
CC = gcc
CFLAGS = -std=c23 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 -I$(pqueue_dir)
vpath %.c $(pqueue_dir)
vpath %.h $(pqueue_dir)
objects = pqueue.o main.o
executable = exemple
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable) 

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

pqueue.o: pqueue.c pqueue.h
main.o: main.c pqueue.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)