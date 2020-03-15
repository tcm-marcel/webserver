#pragma once

#include "Http.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <ext/stdio_filebuf.h>


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
   : connection_fd_(connection_fd), client_address_(client_address), 
     output_filebuf_(connection_fd_, std::ios::in), output_stream_(&output_filebuf_) {};
  
  HttpRequestHeader readRequestHeader();
  void sendResponse(HttpResponseHeader& header, std::string& content);
  void sendEmptyResponse(HttpResponseHeader& header)
  {
    std::string empty = "";
    sendResponse(header, empty);
  }
  
private:
  int connection_fd_;
  socket_address client_address_;
  
  /// Input buffer
  std::string input_buffer_;
  
  /// Output stream, wrapper around socker file descriptor
  __gnu_cxx::stdio_filebuf<char> output_filebuf_;
  std::ostream output_stream_;
  
  std::string readLine();
  
  void sendResponseHeader(HttpResponseHeader& header);
  void flushAndCloseConnection();
};


} // namespace webserver

