#
# Makefile des tests du projet.
#

include ../variables.mk

#
# Organisation des sources.
#

# Packages.
PACKAGES = sender proxyLogger proxyGUI postman dispatcher driverCAN

# Wrap des méthodes pour bouchonnage.$
# LDWRAP  += -Wl,--wrap=mod1_fct4

# V1 : Bouchonnage avec le wrap : 
# LDWRAP  += -Wl,--wrap=mod1_fct_st1

# M�thode bouchonn�e pour des besoins internes � un module
# CCFLAGS += -D_WRAP_MOD1_FCT4

# V1 : Bouchonnage avec le wrap : 
# CCFLAGS += -D_WRAP_MOD1_FCT_ST1

# V2 : Bouchonnage avec la reference faible
# CCFLAGS += -D_MOCK_MOD1_FCT_ST1

# Un niveau de package est accessible.
SRC = $(wildcard */*.c)		
OBJ = $(SRC:.c=.o)

# Point d'entrée du programme.
MAIN = main_test.c

# Gestion automatique des dépendances.
DEP = $(MAIN:.c=.d)

# Executable à générer
EXEC = ../bin/CANgateway_test.out

# Inclusion depuis le niveau du package.
CCFLAGS += -I../src -I$(CMOCKA)/include 

# Liens pour les tests.
LDFLAGS += -L$(CMOCKA)/lib 

# Utilisation de la librairie static CMocka
LDFLAGS += -lcmocka-static 

#
# Règles du Makefile.
#

# Compilation.
all: 
	@for p in $(PACKAGES); do (cd $$p && $(MAKE) $@); done
	@$(MAKE) $(EXEC)

$(EXEC): $(OBJ) $(MAIN)
	$(CC) $(CCFLAGS) $(LDWRAP) $(OBJ) $(MAIN) -MF $(DEP) -o $(EXEC) $(LDFLAGS)

# Nettoyage.
.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p && $(MAKE) $@); done
	@rm -f $(DEP)

-include $(DEP)

