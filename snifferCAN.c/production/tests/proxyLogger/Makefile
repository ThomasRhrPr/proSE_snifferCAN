#
# Makefile racine pour les tests du projet.
#

include ../../variables.mk

#
# Organisation des sources.
#
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Inclusion depuis le niveau du package.
CCFLAGS += -I.. -I../../src
CCFLAGS += -I. -I$(CMOCKA)/include

#
# Règles du Makefile.
#

# Compilation.
all: $(OBJ)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@
	
# Nettoyage.
.PHONY: clean

clean:
	@rm -f $(OBJ) $(DEP)

-include $(DEP)

