# compiler options
CC = gcc
CFLAGS = -Wall -g
# path
TARGETDIR = ../bin/
OBJECTDIR = ../obj/
LOGDIR    = ../logs/

TARGET = $(addprefix $(TARGETDIR), myshell)
SOURCES = shell_modoki.c tokenizer.c parser.c debug_functions.c history.c \
		  symbol_table.c executor.c printer.c parse_tree_nodes.c \
		  arithmetic_calculator.c
OBJECTS = $(addprefix $(OBJECTDIR), $(SOURCES:.c=.o))

all: $(TARGETDIR) $(OBJECTDIR) $(LOGDIR) $(TARGET)

$(OBJECTDIR)%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^  -o $@

$(TARGETDIR):
	mkdir ../bin

$(OBJECTDIR):
	mkdir ../obj

$(LOGDIR):
	mkdir ../logs

clean:
	rm -rf $(OBJECTDIR) $(TARGETDIR)
