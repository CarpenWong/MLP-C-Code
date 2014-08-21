Object = MLPTest
all: $(Object)

# Which compiler
CC = gcc

# Where to install
#INSTDIR = /usr/local/bin
#MANDIR = /usr/local/man/man1

#subdirectory
SUBDIRECTORY = ./src

# Where are include files kept
INCLUDE = .

# Options for development
CFLAGS = -g -Wall -ansi

# Options for release
# CFLAGS = -O -Wall -ansi

#built-in rule to management libraries
#.c.a:
#$(CC) -c $(CFLAGS) $<
#$(AR) $(ARFLAGS) $@ $*.o
#
# Local Libraries
LIBS = libmlp.a

$(Object): $(Object).o $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

#this a sub-directory
.c.o:
	@$(CC) $(CFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)

$(LIBS): $(patsubst %.c,%.o,$(wildcard $(SUBDIRECTORY)/*.c))
	@$(AR) $(ARFLAGS) $(LIBS) $?

.PHONY : clean
clean:
	@-rm $(Object).o $(wildcard $(SUBDIRECTORY)/*.o) $(LIBS) $(Object)
