NAME= minishell
CC= cc
FLAGS= -Wall -Wextra -Werror
INCLUDE= include/minishell.h
INCLUDE_READLINE= -lreadline
LIBFT= libft
SRC_DIR = src/
SRC_OBJ = obj/
SRC = $(SRC_DIR)env.c \
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
	make -C $(LIBFT)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(INCLUDE_READLINE) $(LIBFT)/libft.a -g3

$(SRC_OBJ)%.o: $(SRC_DIR)%.c
	mkdir -p $(SRC_OBJ)
	$(CC) $(FLAGS) -I$(LIBFT) -I$(INCLUDE) -c $< -o $@ -g3

clean:
	rm -rf $(SRC_OBJ)
	make -C $(LIBFT) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re
