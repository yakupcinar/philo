NAME        = philo

CC          = cc
# Makefile içine ekle
CFLAGS 		= -Wall -Wextra -Werror -pthread 
RM          = rm -f

# Libft dizini ve kütüphane dosyası
LIBFT_DIR   = ft_libft
LIBFT       = $(LIBFT_DIR)/libft.a

# Projenin kaynak dosyaları
SRCS        = main.c init.c routine.c simulation.c utils.c leak.c
OBJS        = $(SRCS:.c=.o)

# Renklendirme (Terminal çıktısını güzelleştirmek için opsiyonel)
GREEN       = \033[1;32m
YELLOW      = \033[1;33m
RESET       = \033[0m

all: $(LIBFT) $(NAME)

# Önce Libft kütüphanesini kendi Makefile'ı ile derliyoruz
$(LIBFT):
	@echo "$(YELLOW)Compiling Libft...$(RESET)"
	@make -C $(LIBFT_DIR)

# Ana programı derleme ve Libft ile bağlama
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)Philosophers successfully compiled!$(RESET)"

%.o: %.c philo.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	@$(RM) $(OBJS)
	@echo "$(YELLOW)Object files cleaned.$(RESET)"

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME)
	@echo "$(YELLOW)Executable and libraries cleaned.$(RESET)"

re: fclean all

.PHONY: all clean fclean re