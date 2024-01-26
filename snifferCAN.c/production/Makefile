# Nom de l'execécutable
EXEC = bin/CANgateway.out
TEST = bin/CANgateway_test.out

# Cible par défaut
TARGET = default

# Script de transfert du binaire sur la Raspberry Pi
TRANSFER_SCRIPT = transfert_bin.sh

SUBDIRS = src tests

# Compilation récursive.
all: 
	@for i in $(SUBDIRS); do (cd $$i; make $@); done
	@if [ "$(TARGET)" = "raspberry" ]; then \
		bash $(TRANSFER_SCRIPT) $(EXEC); \
	fi
	@if [ "$(TARGET)" = "raspberry" ]; then \
		bash $(TRANSFER_SCRIPT) $(TEST); \
	fi

# Nettoyage.
.PHONY: clean

clean:
	@for i in $(SUBDIRS); do (cd $$i; make $@); done
	@rm -f $(EXEC) $(TEST)

