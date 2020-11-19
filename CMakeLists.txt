cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv
        main.cpp
        VirtualServer.hpp
        Route.hpp
        Request.hpp
        WebServ.hpp
        ParseConfigFile.hpp
        libft/ft_memset.cpp)