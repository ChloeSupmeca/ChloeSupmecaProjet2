# ============================================================
#  Makefile — Cabinet Dentaire
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -Wshadow -std=c11 -g
TARGET  = cabinet_dentaire

SRCS    = main.c partie.c utils.c actions.c jeu.c
OBJS    = $(SRCS:.c=.o)

# ---- Règle principale ----
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# ---- Compilation de chaque .c ----
main.o:    main.c    types.h partie.h jeu.h
	$(CC) $(CFLAGS) -c $<

partie.o:  partie.c  types.h utils.h partie.h
	$(CC) $(CFLAGS) -c $<

utils.o:   utils.c   types.h utils.h
	$(CC) $(CFLAGS) -c $<

actions.o: actions.c types.h utils.h partie.h actions.h
	$(CC) $(CFLAGS) -c $<

jeu.o:     jeu.c     types.h partie.h actions.h utils.h jeu.h
	$(CC) $(CFLAGS) -c $<

# ---- Lancer le jeu ----
run: all
	./$(TARGET)

# ---- Nettoyage ----
clean:
	rm -f $(OBJS) $(TARGET)

# ---- Nettoyage complet (incluant sauvegardes) ----
mrproper: clean
	rm -f sauvegarde.dat scores.txt

.PHONY: all run clean mrproper
