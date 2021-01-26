NAME	=	WebServv
FUNC	=	bytes/bytes.cpp \
            Client/Client.cpp \
            HttpObject/HttpObject.cpp \
            HttpRequest/HttpRequest.cpp \
            HttpResponse/HttpResponse.cpp \
            libft/ft_atoi.cpp \
            libft/ft_decode64base.cpp \
            libft/ft_getdate.cpp \
            libft/ft_getfile.cpp \
            libft/ft_getline.cpp \
            libft/ft_hton.cpp \
            libft/ft_itoa.cpp \
            libft/ft_localtime.cpp \
            libft/ft_memcopy.cpp \
            libft/ft_memset.cpp \
            libft/ft_tolower.cpp \
            Location/Location.cpp \
            ParseConfigFile/ParseConfigFile.cpp \
            ThreadPool/ThreadPool.cpp \
            VirtualServer/VirtualServer.cpp \
            WebServ/WebServ.cpp \
            WebServ/WebServ_handlers.cpp \
            WebServ/WebServ_page_generators.cpp \
            WebServ/WebServ_workers.cpp \
            WebServ/WebServ_cgi.cpp \
            main.cpp


INC		=	-I./bytes/ \
            -I./Client/ \
            -I./HttpObject/ \
            -I./HttpRequest/ \
            -I./HttpResponse/ \
            -I./includes/ \
            -I./Location/ \
            -I./ParseConfigFile/ \
            -I./ThreadPool/ \
            -I./VirtualServer/ \
            -I./WebServ/

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