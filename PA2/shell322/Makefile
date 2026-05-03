# compiler and flags
CC     = gcc
CFLAGS = -Wall -Wextra -g

# source files and output
SRCS = main.c parser.c builtins.c executor.c history.c
OBJS = $(SRCS:.c=.o)
TARGET = shell322

# default target - builds everything
all: $(TARGET)

# link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# compile each .c file into .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# rebuild from scratch
re: clean all

# usage:
#make          
#make clean    
#make re       
#./shell322  