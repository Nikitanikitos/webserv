cmake_minimum_required(VERSION 3.16.5)
project(webserv)

set(CMAKE_CXX_STANDARD 20)

add_executable(webserv
        main.cpp

		libft/ft_memset.cpp
		libft/ft_hton.cpp
		libft/ft_atoi.cpp

		WebServ/WebServ.cpp
		VirtualServer/VirtualServer.cpp
		Route/Route.cpp
		ParseConfigFile/ParseConfigFile.cpp
		Request/Request.cpp
		)

include_directories(
		includes
		Route
		VirtualServer
		WebServ
		ParseConfigFile
		Request
		)
