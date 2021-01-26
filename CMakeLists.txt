cmake_minimum_required(VERSION 3.16.5)
project(WebServ)

set(CMAKE_CXX_STANDARD 20)

add_executable(WebServ
		src/main.cpp

		src/bytes/bytes.cpp
		src/libft/ft_memset.cpp
		src/libft/ft_hton.cpp
		src/libft/ft_atoi.cpp
		src/libft/ft_localtime.cpp
		src/libft/ft_getdate.cpp
		src/libft/ft_getline.cpp
		src/libft/ft_getfile.cpp
		src/libft/ft_itoa.cpp
		src/libft/ft_tolower.cpp
		src/libft/ft_decode64base.cpp
		src/libft/ft_memcopy.cpp
		src/libft/ft_strdup.cpp
		src/libft/ft_strlen.cpp

        src/WebServ/WebServ.cpp
        src/WebServ/WebServ_workers.cpp
        src/WebServ/WebServ_handlers.cpp
        src/WebServ/WebServ_page_generators.cpp

		src/VirtualServer/VirtualServer.cpp
		src/Location/Location.cpp

		src/ParseConfigFile/ParseConfigFile.cpp

		src/HttpRequest/HttpRequest.cpp
		src/HttpResponse/HttpResponse.cpp
		src/HttpObject/HttpObject.cpp

		src/Client/Client.cpp
		src/ThreadPool/ThreadPool.cpp
        src/WebServ/WebServ_cgi.cpp
		)

include_directories(
		src/includes
		src/Location
		src/VirtualServer
        src/WebServ
		src/ParseConfigFile
		src/HttpRequest
		src/HttpResponse
		src/Client
		src/ThreadPool
		src/bytes
		src/HttpObject
		)

target_link_libraries(WebServ pthread)
