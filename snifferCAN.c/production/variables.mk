# Outils différents selon la plateforme de développement :
ifeq ($(TARGET), raspberry) # pour la cible Raspberry.

# Framework de test CMocka.
export CMOCKA = /home/elisa/library/cmocka/cmocka-raspberry
# exemple : export CMOCKA = /home/matth/Logiciels/CMocka/cmocka-1.1.5-raspberry/

# Répertoire d'installation des outils de développement pour Raspberry Pi (à adapter, si besoin).
RASPBERRY_TOOLS := /home/elisa/git/se2024-b1.c/production/tools
# RASPBERRY_TOOLS := /home/thomas/Documents/ProSE/se2024-b1.c/production/tools

# Compilateur croisé à utiliser (à adapter)
RASPBERRY_GCC := $(RASPBERRY_TOOLS)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc
# $(RASPBERRY_TOOLS)/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

# Sysroot (à adapter)
RASPBERRY_SYSROOT := $(RASPBERRY_TOOLS)/rootfs_bplus/

# Compilateur :
CC := $(RASPBERRY_GCC)

# Options de compilation :
# CCFLAGS += --sysroot=$(RASPBERRY_SYSROOT)
CCFLAGS += -std=c99 -Wall -I. -pedantic -D_POSIX_C_SOURCE=200809L -g -DDEBUG -MMD -MP

# Editeur des liens.
LDFLAGS := -pthread -lrt 
# LDFLAGS += --sysroot=$(RASPBERRY_SYSROOT) -lrt

############################################################################################################################
else # pour le pc de développement.

# Framework de test CMocka (à adapter) :
export CMOCKA = /home/elisa/library/cmocka/cmocka-x86
# exemple : export CMOCKA = /home/matth/Logiciels/CMocka/cmocka-1.1.5-x86_64/

# Compilateur :
LOCAL_GCC := gcc

CC := $(LOCAL_GCC)

# Options de compilation.
CCFLAGS += -std=c99 -Wall -I. -pedantic -D_POSIX_C_SOURCE=200809L -g -DDEBUG -MMD -MP

# Editeur des liens.
LDFLAGS := -pthread -lrt 

endif