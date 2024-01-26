#
# CANgateway - facteurServeur
#
# @author Elisa DECLERCK
#

include ../../variables.mk

#Organisation des sources
SRC = 	$(wildcard *.c) 

OBJ = 	$(SRC:.c=.o)

DEP = $(SRC:.c=.d)

#Regles de compilations
all: $(OBJ)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

#Clean Project
clean:
	$(RM) *.o $(EXEC) $(DEP)

-include $(DEP)
