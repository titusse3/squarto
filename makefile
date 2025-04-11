menu_dir = menu/
rules_menu_dir = menu/rules_menu/
utils_menu_dir = menu/utils_menu/
rlights_dir = rlights/
utils_dir = utils/
quarto_dir = quarto/
mbck_dir = mbck/
solver_dir = solver/
heuristic_dir = solver/heuristic
holdall_dir = solver/holdall
ktree_dir = solver/ktree
pqueue_dir = solver/pqueue
game_view_dir = game_view/
CC = gcc
CFLAGS = -std=c23 \
	-I$(menu_dir) -I$(rlights_dir) -I$(utils_dir) -I$(quarto_dir) -I$(mbck_dir) \
	-I$(rules_menu_dir) -I$(utils_menu_dir) -I$(holdall_dir) -I$(ktree_dir) \
	-I$(pqueue_dir) -I$(solver_dir) -I$(heuristic_dir) -I$(game_view_dir) -DQUARTO_EXT=1
vpath %.c $(menu_dir) $(rlights_dir) $(utils_dir) $(quarto_dir) $(mbck_dir) \
	$(rules_menu_dir) $(utils_menu_dir) $(holdall_dir) $(ktree_dir) \
	$(pqueue_dir) $(solver_dir) $(heuristic_dir) $(game_view_dir)
vpath %.h $(menu_dir) $(rlights_dir) $(utils_dir) $(quarto_dir) $(mbck_dir) \
	$(rules_menu_dir) $(utils_menu_dir) $(holdall_dir) $(ktree_dir) \
	$(pqueue_dir) $(solver_dir) $(heuristic_dir) $(game_view_dir)
objects = menu.o rules_menu.o utils_menu.o main.o rlights.o utils.o mbck.o \
	holdall.o ktree.o pqueue.o quarto.o solver.o heuristic.o game_view.o
executable = game
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable) test

test: $(executable)
	./$(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

mbck.o: mbck.c mbck.h
game_view.o: game_view.c game_view.h menu.h utils_menu.h solver.h heuristic.h
utils_menu.o: utils_menu.c utils_menu.h menu.h
rules_menu.o: rules_menu.c rules_menu.h menu.h utils_menu.h
menu.o: menu.c menu.h rlights.h utils.h rules_menu.h utils_menu.h
main.o: main.c menu.h rlights.h utils.h quarto.h utils_menu.h
heuristic.o: heuristic.c heuristic.h quarto.h solver.h
rlights.o: rlights.h rlights.c
utils.o: utils.c utils.h rlights.h
holdall.o: holdall.c holdall.h
pqueue.o: pqueue.c pqueue.h
ktree.o: ktree.c ktree.h
solver.o: solver.c solver.h quarto.h holdall.h ktree.h pqueue.h
quarto.o: quarto.c quarto.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
