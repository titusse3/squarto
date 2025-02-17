CC = gcc
CFLAGS = -std=c23 \
  # -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  # -O2
objects = quarto.o quarto_test.o
executable = quarto_test
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
