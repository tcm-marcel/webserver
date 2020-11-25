// (c) Marcel Kost 2020

#include "ClientConnection.hpp"
#include "Util.hpp"

#include <cerrno>
#include <cstring>
#include <sstream>
#include <iostream>
#include <unistd.h>


namespace webserver {


HttpRequestHeader ClientConnection::readRequestHeader()
{
  input_ = receive();
  std::istringstream input_stream = std::istringstream(input_);
  
  HttpRequestHeader header;
  std::string method;
  std::string protocolVersion;
  std::string encodedPath;
  
  // Read Request Line
  input_stream >> method >> encodedPath >> protocolVersion;
  
  header.method = httpMethodFromString(method);
  header.path = urlDecode(encodedPath);
  
  if (header.method == HttpRequestHeader::Method::Unknown)
    throw HttpError{501, "Not Implemented", {}};
  
  if (protocolVersion != protocolVersion)
    throw HttpError{505, "HTTP Version Not Supported", {}};
  
  do
  {
    std::string name, value;
    std::getline(input_stream, name, ':');
    std::getline(input_stream, value);

    // field names are ase insensitive
    header.fields[toLowercase(name)] = value;
  }
  while (input_stream.good());

  return header;
}


void ClientConnection::sendResponse(HttpResponseHeader& header, const std::string& content)
{
  HttpHeaderFields additional_fields{{"Content-Length", std::to_string(content.size())}};
  
  sendResponseHeader(header, additional_fields);
  send(content);
  flushAndCloseConnection();
}


void ClientConnection::sendResponse(HttpResponseHeader& header, int fd, size_t size)
{
  HttpHeaderFields additional_fields{{"Content-Length", std::to_string(size)}};
  
  sendResponseHeader(header, additional_fields);
  send(fd, size);
  flushAndCloseConnection();
}


void ClientConnection::sendEmptyResponse(HttpResponseHeader& header)
{
  HttpHeaderFields additional_fields{{"Content-Length", "0"}};
  
  sendResponseHeader(header, additional_fields);
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


void ClientConnection::send(const std::string& output)
{
  auto write_size = write(connection_fd_, output.data(), output.size());
  
  if (write_size < 0) {
    throw NetworkError(std::string("Error writing data to client socket: ") + std::strerror(errno));
  } else if (static_cast<size_t>(write_size) != output.size()) {
    throw NetworkError(std::string("Wrong number of bytes written to client socket: ") + std::strerror(errno));
  }
}

void ClientConnection::send(int fd, size_t size)
{
  size_t overall_read_size = 0;
  const size_t buffer_size = 4096;
  uint8_t buffer[buffer_size];
  
  do {
    auto read_size = read(fd, buffer, buffer_size);
    overall_read_size += read_size;
    
    auto write_size = write(connection_fd_, buffer, read_size);
    
    if (write_size < 0) {
      throw NetworkError(std::string("Error writing data to client socket: ") + std::strerror(errno));
    } else if (write_size != read_size) {
      throw NetworkError(std::string("Wrong number of bytes written to client socket: ") + std::strerror(errno));
    }
  } while (overall_read_size < size);
}


void ClientConnection::sendResponseHeader(HttpResponseHeader& header, HttpHeaderFields& additional_fields)
{
  std::ostringstream output_stream;
  
  output_stream << implementedProtocolVersion << " " << header.statusCode << " " << header.statusText << "\n";
  for (auto const& [key, value] : additional_fields) {
    output_stream << key << ": " << value << "\n";
  }
  for (auto const& [key, value] : header.fields) {
    output_stream << key << ": " << value << "\n";
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

