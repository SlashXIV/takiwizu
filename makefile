#makefile
#for object : $(CC) -c $<
#for execute : $(CC) $(CFLAGS) $^ $@  <lib if needed> 
 
CC= gcc
CFLAGS= -Wall -std=c99
LDFLAGS= -L./
AR= ar






#Cleaning

.PHONY : clean
clean :
	rm -f *.o
