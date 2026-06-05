NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
FILES = ft_atoi.c ft_isprint.c ft_putchar_fd.c ft_striteri.c ft_strnstr.c \
ft_bzero.c ft_itoa.c ft_putendl_fd.c ft_strjoin.c ft_strrchr.c \
ft_calloc.c ft_memchr.c ft_putnbr_fd.c ft_strlcat.c ft_strtrim.c \
ft_isalnum.c ft_memcmp.c ft_putstr_fd.c ft_strlcpy.c ft_substr.c \
ft_isalpha.c ft_memcpy.c ft_split.c ft_strlen.c ft_tolower.c \
ft_isascii.c ft_memmove.c ft_strchr.c ft_strmapi.c ft_toupper.c \
ft_isdigit.c ft_memset.c ft_strdup.c ft_strncmp.c \
ft_printf.c ft_putchar.c ft_putstr.c ft_putnbr.c ft_put_unsigned.c ft_put_hex.c ft_pointer.c \
get_next_line.c gnl_strlen.c gnl_strchr.c gnl_strdup.c gnl_strjoin.c gnl_substr.c\

FILES_BONUS = ft_lstadd_back.c ft_lstdelone.c ft_lstmap.c\
ft_lstadd_front.c ft_lstiter.c ft_lstnew.c\
ft_lstclear.c ft_lstlast.c ft_lstsize.c\

OFILES = $(FILES:.c=.o)

OFILES_BONUS = $(FILES_BONUS:.c=.o)

$(NAME): $(OFILES)
	$(AR) $(NAME) $(OFILES) 

all: $(NAME)

bonus: $(OFILES) $(OFILES_BONUS)
	$(AR) $(NAME) $(OFILES) $(OFILES_BONUS)

clean:
	rm -f $(OFILES) $(OFILES_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all 

.PHONY: all bonus clean fclean re