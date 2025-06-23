# compiler options
CC = gcc
CFLAGS = -Wall -g
# path
TARGETDIR = ./bin/
OBJECTDIR = ./obj/

TARGET = $(addprefix $(TARGETDIR), myshell)
SOURCES = shell_modoki.c tokenizer.c 
OBJECTS = $(addprefix $(OBJECTDIR), $(SOURCES:.c=.o))

all: $(TARGETDIR) $(OBJECTDIR) $(TARGET)

$(OBJECTDIR)%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^  -o $@

$(TARGETDIR):
	mkdir ./bin

$(OBJECTDIR):
	mkdir ./obj

clean:
	rm -rf $(OBJECTDIR) $(TARGETDIR)
