# compiler options
CC = gcc
CFLAGS = -Wall -g
# path
TARGETDIR = ../bin/
OBJECTDIR = ../obj/

TARGET = $(addprefix $(TARGETDIR), myshell)
SOURCES = shell_modoki.c tokenizer.c parser.c debug_functions.c history.c \
		  symbol_table.c executor.c printer.c parse_tree_nodes.c
OBJECTS = $(addprefix $(OBJECTDIR), $(SOURCES:.c=.o))

all: $(TARGETDIR) $(OBJECTDIR) $(TARGET)

$(OBJECTDIR)%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^  -o $@

$(TARGETDIR):
	mkdir ../bin

$(OBJECTDIR):
	mkdir ../obj

$(OBJECTDIR):
	mkdir ../logs

clean:
	rm -rf $(OBJECTDIR) $(TARGETDIR)
