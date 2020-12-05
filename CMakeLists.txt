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

		WebServ/WebServ.cpp
		WebServ/WebServ_workers.cpp
		WebServ/WebServ_default_handler.cpp

		VirtualServer/VirtualServer.cpp
		Location/Location.cpp
		ParseConfigFile/ParseConfigFile.cpp
		Request/Request.cpp
        Response/Response.cpp

		HttpObject/HttpObject.cpp
		ThreadPool/ThreadPool.cpp

		exceptions/exceptions.cpp
		)

include_directories(
		includes
		exceptions
        Location
		VirtualServer
		WebServ
		ParseConfigFile
		Request
		Response
		HttpObject
		ThreadPool
		)
