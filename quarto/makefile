CC = gcc
CFLAGS = -std=c23 \
  # -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  # -O2
objects = quarto.o main.o
executable = main
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

quarto.o: quarto.c quarto.h
main.o: main.c quarto.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
