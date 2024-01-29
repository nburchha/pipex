NAME = pipex
CC = cc
CFLAGS = -Wall -Werror -Wextra #-g -fsanitize=address
LIB = ./libs42/libs42.a
SRC_FILES = main.c utils.c
OBJS = $(addprefix .obj/, $(SRC_FILES:.c=.o))
GREEN=\033[32m
RED=\033[31m
RESET=\033[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIB)
	@echo "$(GREEN)Linking$(RESET)"
	@$(CC) $(OBJS) $(LIB) -o $(NAME) $(CFLAGS)

.obj/%.o: src/%.c
	@echo "$(GREEN)Compiling $<$(RESET)"
	@mkdir -p .obj
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	@cd libs42 && make all
clean:
	@echo "$(RED)Removing $(OBJS)$(RESET)"
	@rm -f $(OBJS)
	@cd libs42 && make fclean
fclean: clean
	@echo "$(RED)Removing $(NAME)$(RESET)"
	@rm -f $(NAME) $(BONUS_NAME)
re: fclean $(NAME)

.PHONY: clean fclean re all bonus