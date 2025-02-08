menu_dir = menu/
rlights_dir = rlights/
CC = gcc
CFLAGS = -std=c23 \
	-I$(menu_dir) -I$(rlights_dir)
vpath %.c $(menu_dir) $(rlights_dir)
vpath %.h $(menu_dir) $(rlights_dir)
objects = menu.o main.o
executable = game
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable) test

test: $(executable)
	clear && ./$(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c menu.h
	$(CC) $(CFLAGS) -c $< -o $@

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
