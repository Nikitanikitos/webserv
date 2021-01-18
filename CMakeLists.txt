cmake_minimum_required(VERSION 3.16.5)
project(webserv)

set(CMAKE_CXX_STANDARD 20)

add_executable(webserv
        main.cpp

		bytes/bytes.cpp
		libft/ft_memset.cpp
		libft/ft_hton.cpp
		libft/ft_atoi.cpp
		libft/ft_localtime.cpp
		libft/ft_getdate.cpp
		libft/ft_getline.cpp
		libft/ft_getfile.cpp
		libft/ft_itoa.cpp
		libft/ft_tolower.cpp
		libft/ft_decode64base.cpp
		libft/ft_memcopy.cpp

		WebServ/WebServ.cpp
		WebServ/WebServ_workers.cpp
		WebServ/WebServ_handlers.cpp
		WebServ/WebServ_page_generators.cpp

		VirtualServer/VirtualServer.cpp
		Location/Location.cpp

		ParseConfigFile/ParseConfigFile.cpp

		HttpRequest/HttpRequest.cpp
		HttpResponse/HttpResponse.cpp
		HttpObject/HttpObject.cpp

		Client/Client.cpp
		ThreadPool/ThreadPool.cpp
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

target_link_libraries(webserv pthread)
