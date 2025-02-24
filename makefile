menu_dir = menu/
rlights_dir = rlights/
utils_dir = utils/
quarto_dir = quarto/
mbck_dir = mbck/
CC = gcc
CFLAGS = -std=c23 \
	-I$(menu_dir) -I$(rlights_dir) -I$(utils_dir) -I$(quarto_dir) -I$(mbck_dir)
vpath %.c $(menu_dir) $(rlights_dir) $(utils_dir) $(quarto_dir) $(mbck_dir)
vpath %.h $(menu_dir) $(rlights_dir) $(utils_dir) $(quarto_dir) $(mbck_dir)
objects = menu.o main.o rlights.o utils.o mbck.o quarto.o
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

mbck.o: mbck.c mbck.h
menu.o: menu.c menu.h rlights.h utils.h
main.o: main.c menu.h rlights.h utils.h quarto.h
rlights.o: rlights.h rlights.c
utils.o: utils.c utils.h rlights.h
quarto.o: quarto.c quarto.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
