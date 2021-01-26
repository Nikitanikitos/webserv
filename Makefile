NAME	=	webserv
FUNC	=	src/bytes/bytes.cpp \
            src/Client/Client.cpp \
            src/HttpObject/HttpObject.cpp \
            src/HttpRequest/HttpRequest.cpp \
            src/HttpResponse/HttpResponse.cpp \
            src/libft/ft_atoi.cpp \
            src/libft/ft_decode64base.cpp \
            src/libft/ft_getdate.cpp \
            src/libft/ft_getfile.cpp \
            src/libft/ft_getline.cpp \
            src/libft/ft_hton.cpp \
            src/libft/ft_itoa.cpp \
            src/libft/ft_localtime.cpp \
            src/libft/ft_memcopy.cpp \
            src/libft/ft_memset.cpp \
            src/libft/ft_tolower.cpp \
            src/libft/ft_strdup.cpp \
            src/Location/Location.cpp \
            src/ParseConfigFile/ParseConfigFile.cpp \
            src/ThreadPool/ThreadPool.cpp \
            src/VirtualServer/VirtualServer.cpp \
            src/WebServ/WebServ.cpp \
            src/WebServ/WebServ_handlers.cpp \
            src/WebServ/WebServ_page_generators.cpp \
            src/WebServ/WebServ_workers.cpp \
            src/WebServ/WebServ_cgi.cpp \
            src/main.cpp


INC		=	-I./src/bytes/ \
            -I./src/Client/ \
            -I./src/HttpObject/ \
            -I./src/HttpRequest/ \
            -I./src/HttpResponse/ \
            -I./src/includes/ \
            -I./src/Location/ \
            -I./src/ParseConfigFile/ \
            -I./src/ThreadPool/ \
            -I./src/VirtualServer/ \
            -I./src/WebServ/

CC		=	clang++
FLAGS	=	-Wall -Wextra -Werror -pthread
OBJ		=	$(FUNC:.cpp=.o)

all: $(NAME)
	@echo "\033[32m[+] Make completed\033[0m"

%.o: %.cpp
	@$(CC) $(FLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(INC) $(LFT_FLG) -o $(NAME)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re