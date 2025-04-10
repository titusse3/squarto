ktree_dir = ../
CC = gcc
CFLAGS = -std=c23 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 -I$(ktree_dir)
vpath %.c $(ktree_dir)
vpath %.h $(ktree_dir)
objects = ktree.o main.o
executable = exemple
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable) 

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

ktree.o: ktree.c ktree.h
main.o: main.c ktree.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)