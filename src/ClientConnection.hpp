// (c) Marcel Kost 2020

#pragma once

#include "Http.hpp"

#include <string>
#include <netinet/in.h>


namespace webserver {


class ClientConnection
{
public:
  using socket_address = struct sockaddr_in;
  
  class NetworkError : public std::runtime_error
  {
  	using std::runtime_error::runtime_error;
  };
  
  ClientConnection(int connection_fd, socket_address client_address) 
   : connection_fd_(connection_fd), client_address_(client_address) {}
  
  HttpRequestHeader readRequestHeader();
  void sendResponse(HttpResponseHeader& header, std::string& content);
  void sendResponse(HttpResponseHeader& header, int fd, size_t size);
  void sendEmptyResponse(HttpResponseHeader& header);
  
private:
  int connection_fd_;
  socket_address client_address_;
  
  std::string input_;
  
  std::string receive() const;
  void send(std::string& output);
  void send(int fd, size_t size);
  
  void sendResponseHeader(HttpResponseHeader& header, std::vector<std::string>& additional_headers);
  void flushAndCloseConnection();
};


} // namespace webserver

