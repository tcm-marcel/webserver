#include "ClientConnection.hpp"

#include <cerrno>
#include <cstring>
#include <unistd.h>


namespace webserver {


HttpRequestHeader ClientConnection::readRequestHeader()
{
  input_ = receive();
  input_stream_ = std::istringstream(input_);
  
  HttpRequestHeader header;
  std::string method;
  std::string protocolVersion;
  
  // Read Request Line
  input_stream_ >> method >> header.path >> protocolVersion;
  
  header.method = httpMethodFromString(method);
  
  if (header.method == HttpRequestHeader::Method::Unknown)
    throw HttpError{501, "Not Implemented", {}};
  
  if (protocolVersion != protocolVersion)
    throw HttpError{505, "HTTP Version Not Supported", {}};
  
  return header;
}


void ClientConnection::sendResponse(HttpResponseHeader& header, std::string& content)
{
  std::vector<std::string> additional_headers{
    "Content-Length: " + std::to_string(content.size()),
    "Content-Type: text/plain"
  };
  
  sendResponseHeader(header, additional_headers);
  send(content);
  flushAndCloseConnection();
}


void ClientConnection::sendEmptyResponse(HttpResponseHeader& header)
{
  std::vector<std::string> additional_headers{"Content-Length: 0"};
  
  sendResponseHeader(header, additional_headers);
  flushAndCloseConnection();
}


std::string ClientConnection::receive() const
{
  const size_t buffer_size = 1024;
  std::string buffer(buffer_size, '\0');
  std::string input;
  
  while(true) {
    auto read_size = read(connection_fd_, buffer.data(), buffer_size);
    
    if (read_size < 0) {
      throw NetworkError(std::string("Error reading data from client socket: ") + std::strerror(errno));
    } else if (read_size == 0) {
      // Socket closed
      return input;
    }
    
    auto pos = buffer.find("\r\n\r\n");
    if (pos != std::string::npos) {
      // Double newline indicates end of header
      input.append(buffer.substr(0, pos));
      return input;
      
      // TODO: Preserve remainder to make it possible to call the function again
    } else {
      // Continue to receive and wait for end or double newline
      input.append(buffer, read_size);
    }
  }
}


void ClientConnection::send(std::string& output)
{
  auto write_size = write(connection_fd_, output.data(), output.size());
  
  if (write_size < 0) {
    throw NetworkError(std::string("Error writing data to client socket: ") + std::strerror(errno));
  } else if (write_size != output.size()) {
    throw NetworkError(std::string("Wrong number of bytes written to client socket: ") + std::strerror(errno));
  }
}


void ClientConnection::sendResponseHeader(HttpResponseHeader& header, std::vector<std::string>& additional_headers)
{
  std::ostringstream output_stream;
  
  output_stream << protocolVersion << " " << header.statusCode << " " << header.statusText << "\n";
  for (std::string& header : additional_headers) {
    output_stream << header << "\n";
  }
  for (std::string& header : header.headers) {
    output_stream << header << "\n";
  }
  
  // Second CTRL shows end of response header
  output_stream << "\n";
  
  auto output = output_stream.str();
  send(output);
}


void ClientConnection::flushAndCloseConnection()
{
  shutdown(connection_fd_, SHUT_RDWR);
  close(connection_fd_);
}


} // namespace webserver

