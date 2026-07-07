NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g

# Directories
SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes
LIBFT_DIR   = libft

# Libft
LIBFT       = $(LIBFT_DIR)/libft.a
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

# Source files (matching your VS Code explorer)
SRCS        = $(SRC_DIR)/main.c \
              $(SRC_DIR)/builtins/builtin_dispatcher.c \
              $(SRC_DIR)/builtins/ft_cd.c \
              $(SRC_DIR)/builtins/ft_echo.c \
              $(SRC_DIR)/builtins/ft_env.c \
              $(SRC_DIR)/builtins/ft_exit.c \
              $(SRC_DIR)/builtins/ft_export.c \
              $(SRC_DIR)/builtins/ft_pwd.c \
              $(SRC_DIR)/builtins/ft_unset.c \
              $(SRC_DIR)/env/env_init.c \
              $(SRC_DIR)/env/env_utils.c \
              $(SRC_DIR)/execution/execute.c \
              $(SRC_DIR)/execution/execute_utils.c \
              $(SRC_DIR)/execution/heredoc.c \
              $(SRC_DIR)/execution/heredoc_utils.c \
              $(SRC_DIR)/execution/path_resolver.c \
              $(SRC_DIR)/execution/redirections.c \
              $(SRC_DIR)/signals/signals.c \
              $(SRC_DIR)/lexer/lexer.c \
              $(SRC_DIR)/parse/parsing.c \
              $(SRC_DIR)/utils/utils.c \

# Object files (translates src/%.c to obj/%.o)
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Include paths
INCLUDES    = -I $(INC_DIR) -I $(LIBFT_DIR)

# Rules
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) -lreadline -o $(NAME)

# Compile objects and create subdirectories dynamically
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile libft using its own Makefile
$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re