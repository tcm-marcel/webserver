// (c) Marcel Kost 2020

#pragma once

#include "ClientConnection.hpp"

#include <stdexcept>


namespace webserver {


class ServerSocket
{
public:
  using socket_address = struct sockaddr_in;
  
  class NetworkError : public std::runtime_error
  {
  	using std::runtime_error::runtime_error;
  };
  
  ServerSocket() : socket_fd_(0) {};
  ~ServerSocket();
  
  void bindAndListen(int port);
  ClientConnection accept();
  
private:
  int socket_fd_;
};


} // namespace webserver

