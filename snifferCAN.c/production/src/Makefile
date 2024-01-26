#
# CANgateway - src - Makefile
#
# @author Elisa DECLERCK
#

#
# Organisation des sources.
#

include ../variables.mk

# Packages du projet 
##### ajouter vos modules si besoin #####
PACKAGES = driverCAN sniffer sender proxyLogger proxyGUI postman dispatcher ui

# Un niveau de package est accessible.
SRC = $(wildcard */*.c)

OBJ = $(SRC:.c=.o)

# Point d'entrée du programme.
MAIN = main.c

# Gestion automatique des dépendances.
DEP = $(MAIN:.c=.d)

# Exécutable à générer.
EXEC = ../bin/CANgateway.out

# Compilation.
all:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@$(MAKE) CCFLAGS="$(CCFLAGS)" LDFLAGS="$(LDFLAGS)" $(EXEC)

$(EXEC): $(OBJ) $(MAIN)
	$(CC) $(CCFLAGS) $(OBJ) $(MAIN) -MF $(DEP) -o $(EXEC) $(LDFLAGS)

# Nettoyage.
.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@rm -f *.o
	@rm -f $(DEP)

-include $(DEP)