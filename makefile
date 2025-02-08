CC = gcc
CFLAGS = -std=c23
objects = main.o
executable = game
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

main.o: main.c menu.h
	$(CC) $(CFLAGS) -c $< -o $@

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
