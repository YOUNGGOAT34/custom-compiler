CC=gcc
FLAGS=-Wall -Wextra 
SRC=main.c \
   parser/parser.c\
   lexer/lexer.c \
	scope/stack.c \
	symbol_table/hashmap.c \
	code_generator/code_generator.c
OBJECTS=$(SRC:.c=.o)

BINARY=main
all:$(BINARY)
$(BINARY):$(OBJECTS)
	$(CC) -o $@ $^
%.o:%.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(BINARY) $(OBJECTS)
