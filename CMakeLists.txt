cmake_minimum_required(VERSION 3.16.5)
project(webservv)

set(CMAKE_CXX_STANDARD 20)

add_executable(webservv
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
		WebServ/WebServ_parsing_request.cpp
		WebServ/WebServ_page_generators.cpp

		VirtualServer/VirtualServer.cpp
		Location/Location.cpp
		ParseConfigFile/ParseConfigFile.cpp
		HttpRequest/HttpRequest.cpp
		HttpResponse/HttpResponse.cpp

		Client/Client.cpp
		ThreadPool/ThreadPool.cpp

		bytes/bytes.cpp
		bytes/bytes.hpp
        HttpObject/HttpObject.cpp
		)

include_directories(
		includes
        Location
		VirtualServer
		WebServ
		ParseConfigFile
		HttpRequest
		HttpResponse
		Client
		ThreadPool
		bytes
		HttpObject
		)

target_link_libraries(webservv pthread)