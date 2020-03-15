#include "ClientConnection.hpp"

#include <cerrno>
#include <cstring>
#include <unistd.h>


namespace webserver {


void ClientConnection::sendResponse(HttpResponseHeader& header, std::string& content)
{
  sendResponseHeader(header);
  output_stream_ << content;
  flushAndCloseConnection();
}


HttpRequestHeader ClientConnection::readRequestHeader()
{
  HttpRequestHeader header;
  std::string method;
  std::string protocolVersion;
  
  // Get Request Line
  std::istringstream input_stream(readLine());
  
  // Read Request Line
  input_stream >> method >> header.path >> protocolVersion;
  
  header.method = httpMethodFromString(method);
  
  if (header.method == HttpRequestHeader::Method::Unknown)
    throw HttpError{501, "Not Implemented", {}};
  
  if (protocolVersion != "HTTP/1.0")
    throw HttpError{505, "HTTP Version Not Supported", {}};
  
  std::cout << method << header.path << std::endl;
  
  return header;
}


std::string ClientConnection::readLine()
{
  const size_t chunk_size = 64;
  std::string buffer = std::move(input_buffer_);
  
  while(1) {
    const std::size_t chunk_begin_index = buffer.size();
    buffer.resize(buffer.size() + chunk_size);
    
    auto status = read(connection_fd_, &buffer[chunk_begin_index], chunk_size);
    
    if (status < 0) {
      throw NetworkError(std::string("Error reading data from client socket: ") + std::strerror(errno));
    } else if (status == 0) {
      throw NetworkError(std::string("Client socket closed unexpectetly: ") + std::strerror(errno));
    }
    
    std::size_t newline_index = buffer.find("\n", chunk_begin_index);
    if (newline_index != std::string::npos) {
      // Keep remainder in input buffer for next call
      input_buffer_ = buffer.substr(newline_index + 1);
      
      // Cut of part after
      buffer.resize(newline_index);
      
      return buffer;
    }
    
  }
}


void ClientConnection::sendResponseHeader(HttpResponseHeader& header)
{
  output_stream_ << header.statusCode << header.statusText << "\n";
  for (std::string& header : header.headers) {
    output_stream_ << header << "\n";
  }
  
  // Second CTRL shows end of response header
  output_stream_ << std::endl;
}


void ClientConnection::flushAndCloseConnection()
{
  output_stream_.flush();
  shutdown(connection_fd_, SHUT_RDWR);
  close(connection_fd_);
}


} // namespace webserver

