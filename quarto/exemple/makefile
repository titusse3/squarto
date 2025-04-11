quarto_dir = ../
CC = gcc
CFLAGS = -std=c23 \
	-O2 -I$(quarto_dir)
  # -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  
vpath %.c $(quarto_dir)
vpath %.h $(quarto_dir)
objects = quarto.o quarto_test.o
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
quarto_test.o: quarto_test.c quarto.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
