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
		Location/Location.cpp
		ParseConfigFile/ParseConfigFile.cpp
		Request/Request.cpp
        Response/Response.cpp Response/Response.hpp libft/ft_localtime.cpp)

include_directories(
		includes
        Location
		VirtualServer
		WebServ
		ParseConfigFile
		Request
		)
