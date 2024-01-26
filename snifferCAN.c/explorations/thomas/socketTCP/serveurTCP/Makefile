# Nom de l'execécutable
EXEC = serveurTCP.out
# Script de transfert du binaire sur la Raspberry Pi
TRANSFER_SCRIPT = transfer_bin.sh

# Répertoire d'installation des outils de développement
# pour Raspberry Pi (à adapter, si besoin).
RASPBERRY_TOOLS = /home/thomas/Documents/ProSE/se2024-b1.c/production/tools
# exemple : RASPBERRY_TOOLS = /home/matth/Logiciels/RaspberryPi/Tools/

# Compilateur croisé à utiliser (à adapter)
RASPBERRY_GCC = $(RASPBERRY_TOOLS)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc
# exemple : RASPBERRY_GCC = $(RASPBERRY_TOOLS)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

# Sysroot (à adapter)
RASPBERRY_SYSROOT = $(RASPBERRY_TOOLS)/rootfs_bplus/
# exemple : RASPBERRY_SYSROOT = $(RASPBERRY_TOOLS)/rootfs_bplus/

# Compilateur.
CC = $(RASPBERRY_GCC)
# CCFLAGS += --sysroot=$(RASPBERRY_SYSROOT)
CCFLAGS += -std=c99 -Wall -pedantic -D_POSIX_C_SOURCE=200809L
# Editeur des liens.
LD = $(RASPBERRY_GCC)
# LDFLAGS += --sysroot=$(RASPBERRY_SYSROOT) -lrt


#Organisation des sources
SRC = 	$(wildcard *.c) 

OBJ = 	$(SRC:.c=.o)


#Regles de compilations
all: $(EXEC) transfer

$(EXEC): $(OBJ)
	$(LD) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

# Transferer le binaire sur la pi (bien paramétrer le script !)
transfer: $(EXEC)
	@bash $(TRANSFER_SCRIPT) $(EXEC)
	
#Clean Project
clean:
	$(RM) *.o $(EXEC)
