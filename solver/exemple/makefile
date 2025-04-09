solver_dir = ../
quarto_dir = ../../quarto
CC = gcc
CFLAGS = -std=c23 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 -I$(solver_dir) -I$(quarto_dir)
vpath %.c $(solver_dir) $(quarto_dir)
vpath %.h $(solver_dir) $(quarto_dir)
objects = solver.o quarto.o main.o
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
solver.o: solver.c solver.h
main.o: main.c solver.h quarto.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)