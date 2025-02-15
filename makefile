menu_dir = menu/
rlights_dir = rlights/
utils_dir = utils/
CC = gcc
CFLAGS = -std=c23 \
	-I$(menu_dir) -I$(rlights_dir) -I$(utils_dir)
vpath %.c $(menu_dir) $(rlights_dir) $(utils_dir)
vpath %.h $(menu_dir) $(rlights_dir) $(utils_dir)
objects = menu.o main.o rlights.o
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

menu.o: menu.c menu.h rlights.h utils.h
main.o: main.c menu.h rlights.h utils.h
rlights.o: rlights.h rlights.c

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
