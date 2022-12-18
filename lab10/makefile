# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -Werror

# define any directories containing header files other than /usr/include
INCLUDES = -I inc/

# define any libraries to link into executable:
LIBS =  -lm -lssl -lcrypto

# define the C source files
SRCS = src/main.cpp \
		src/csv_file.cpp \
		src/user.cpp \


# define the C object files
OBJS = $(SRCS:.c=.o)

# define the executable file
MAIN = out

.PHONY: depend clean

all: clean $(MAIN) run

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

run:
	./$(MAIN)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it