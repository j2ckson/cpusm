# define the install directory
INSTALL-DIR = /usr/local/sbin/

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -pthread

# define any directories containing header files other than /usr/include
INCLUDES =

# define library paths in addition to /usr/lib
LFLAGS =

# define any libraries to link into executable:
LIBS = -lm

# define the C source files
SRCS = cpusm.c

# define the C object files 
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = cpusm

.PHONY: depend clean static install

all:    $(MAIN)

$(MAIN): $(OBJS)
	@echo  building dynamically-linked, platform-independent shared-object executable
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	strip $(MAIN)
	
static:
	@echo  building statically-linked platform-independent executable
	$(CC) -c -fpic -o $(OBJS) $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) --static -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	strip $(MAIN)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

install:
	cp $(MAIN) $(INSTALL-DIR)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
