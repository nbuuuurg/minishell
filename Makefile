MAKEFLAGS += --no-print-directory
NAME= minishell
CC= cc
FLAGS= -Wall -Wextra -Werror
INCLUDE= include
INCLUDE_READLINE= -lreadline
LIBFT= libft
SRC_DIR = src/
SRC_OBJ = obj/
SRC = $(SRC_DIR)builtin.c \
	  $(SRC_DIR)env.c \
	  $(SRC_DIR)error.c \
	  $(SRC_DIR)exec.c \
	  $(SRC_DIR)exit.c \
      $(SRC_DIR)free.c \
	  $(SRC_DIR)init.c \
	  $(SRC_DIR)init2.c \
	  $(SRC_DIR)init3.c  \
      $(SRC_DIR)lexer.c \
	  $(SRC_DIR)lexer2.c \
	  $(SRC_DIR)lexer3.c \
	  $(SRC_DIR)lexer4.c \
	  $(SRC_DIR)main.c \
      $(SRC_DIR)parser.c \
	  $(SRC_DIR)parser2.c \
	  $(SRC_DIR)parser3.c \
	  $(SRC_DIR)print.c \
	  $(SRC_DIR)signals.c \
	  $(SRC_DIR)subshell.c \
	  $(SRC_DIR)tokenizer.c \
	  $(SRC_DIR)utils.c \
	  $(SRC_DIR)utils2.c \
	  $(SRC_DIR)utils3.c

OBJ= $(SRC:$(SRC_DIR)%.c=$(SRC_OBJ)%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@printf "\n"
	@$(MAKE) -C $(LIBFT) --no-print-directory --silent
	@printf "\033[1;32m\033[0m\n"
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(INCLUDE_READLINE) $(LIBFT)/libft.a -g3
	@printf "\033[1;32m[ OK ] Build complete: %s\033[0m\n\n" "$(NAME) 🥳"
# 	@./$(NAME)

$(SRC_OBJ)%.o: $(SRC_DIR)%.c
	@mkdir -p $(SRC_OBJ)
	@printf "\033[0;36mCompiling minishell:\033[0m %-25s" "$<"
	@$(CC) $(FLAGS) -I$(LIBFT) -I$(INCLUDE) -c $< -o $@ -g3 2>/dev/null && printf "\033[1;32m[ OK ]\033[0m\n" || printf "\033[1;31m[ FAIL ]\033[0m\n"

clean:
	@rm -rf $(SRC_OBJ)
	@make -C $(LIBFT) clean
	@printf "\033[1;36mObjects cleaned.\033[0m\n"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT) fclean
	@printf "\033[1;36mArchive cleaned.\033[0m\n"

valgrind:
	@printf "\n"
	@$(MAKE) -C $(LIBFT) --no-print-directory --silent
	@printf "\033[1;32m\033[0m\n"
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(INCLUDE_READLINE) $(LIBFT)/libft.a -g3
	@printf "\033[1;32m[ OK ] Build complete: %s\033[0m\n\n" "$(NAME) 🥳"
	@valgrind -q --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --trace-children=yes --suppressions=./ignore_leak_readline ./$(NAME)

re: fclean all

.PHONY: all clean fclean re

# OBJ= $(SRC:$(SRC_DIR)%.c=$(SRC_OBJ)%.o)

# all: $(NAME)

# $(NAME): $(OBJ)
# 	make -C $(LIBFT)
# 	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(INCLUDE_READLINE) $(LIBFT)/libft.a -g3

# $(SRC_OBJ)%.o: $(SRC_DIR)%.c
# 	mkdir -p $(SRC_OBJ)
# 	$(CC) $(FLAGS) -I$(LIBFT) -I$(INCLUDE) -c $< -o $@ -g3

# clean:
# 	rm -rf $(SRC_OBJ)
# 	make -C $(LIBFT) clean

# fclean: clean
# 	rm -f $(NAME)
# 	make -C $(LIBFT) fclean

# re: fclean all

# .PHONY: all clean fclean re