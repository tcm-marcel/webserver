// (c) Marcel Kost 2020

#include "ServerSocket.hpp"

#include <cerrno>
#include <cstring>
#include <inttypes.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


namespace webserver {


ServerSocket::~ServerSocket() {
	if (socket_fd_ > 0)
	{
    shutdown(socket_fd_, SHUT_RDWR);
    close(socket_fd_);
  }
};


void ServerSocket::bindAndListen(int port)
{
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	
	if (socket_fd_ < 0)
		throw NetworkError(std::string("Error opening socket: ") + std::strerror(errno));
	
	// TCP/IP socket that listens to any interface on port 80
	socket_address server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);
	
	if (bind(socket_fd_, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
		throw NetworkError(std::string("Error binding socket: ") + std::strerror(errno));
	
	// Simple Dispatcher, allow queue length of 5
	listen(socket_fd_, 5);
	
	std::cout << "Listening on port " << port << " (http://localhost:" << port << ")" << std::endl;
}


ClientConnection ServerSocket::accept()
{
	socket_address client_address;
	  socklen_t client_address_size = sizeof(client_address);
	int connection_fd = ::accept(socket_fd_, (struct sockaddr *) &client_address, &client_address_size);
	
	if (connection_fd < 0)
		throw NetworkError(std::string("Error on accept: ") + std::strerror(errno));
	
	return ClientConnection(connection_fd, client_address);
}


} // namespace webserver

