cmake_minimum_required(VERSION 3.16.5)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv
        main.cpp

		libft/ft_memset.cpp
		libft/ft_hton.cpp

		Request.hpp
        ParseConfigFile.hpp
		WebServ/WebServ.cpp
		VirtualServer/VirtualServer.cpp
		Route/Route.cpp
		)

include_directories(includes
		Route
		VirtualServer
		WebServ
		)
