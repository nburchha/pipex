NAME = pipex
CC = cc
CFLAGS = -Wall -Werror -Wextra #-fsanitize=address -g
LIB = ./libs42/libs42.a
SRC_FILES = main.c parse_input.c
OBJS = $(addprefix .obj/, $(SRC_FILES:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) $(LIB)
	@$(CC) $(OBJS) $(LIB) -o $(NAME) $(CFLAGS)

.obj/%.o: src/%.c
	@mkdir -p .obj
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	@cd libs42 && make all
clean:
	@rm -f $(OBJS) $(BOBJS)
	@cd libs42 && make fclean
fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)
re: fclean $(NAME)

.PHONY: clean fclean re all bonus