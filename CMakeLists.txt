cmake_minimum_required(VERSION 3.16.5)
project(webserv)

set(CMAKE_CXX_STANDARD 20)

add_executable(webserv
        main.cpp

		libft/ft_memset.cpp
		libft/ft_hton.cpp
		libft/ft_atoi.cpp
		libft/ft_localtime.cpp
		libft/ft_getdate.cpp
		libft/ft_getline.cpp
		libft/ft_getfile.cpp

		WebServ/WebServ.cpp
		WebServ/WebServ_workers.cpp
		WebServ/WebServ_handlers.cpp

		VirtualServer/VirtualServer.cpp
		Location/Location.cpp
		ParseConfigFile/ParseConfigFile.cpp
		Request/Request.cpp
		Response/Response.cpp

		Client/Client.cpp
		ThreadPool/ThreadPool.cpp

        exceptions/ResponseException.cpp
		WebServ/WebServ_parsing_request.cpp
        WebServ/WebServ_page_generators.cpp)

include_directories(
		includes
		exceptions
        Location
		VirtualServer
		WebServ
		ParseConfigFile
		Request
		Response
		Client
		ThreadPool
		)

target_link_libraries(webserv pthread)