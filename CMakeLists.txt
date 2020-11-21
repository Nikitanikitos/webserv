cmake_minimum_required(VERSION 3.16.5)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv
        main.cpp
        VirtualServer.hpp
        Route.hpp
        Request.hpp
        WebServ.hpp
        ParseConfigFile.hpp

        libft/ft_memset.cpp
		libft/ft_hton.cpp
		WebServ.cpp WebServ.hpp)
