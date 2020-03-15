// (c) Marcel Kost 2020

#pragma once

#include <stdexcept>
#include <string>
#include <vector>


namespace webserver {


using httpStatusCode = unsigned int;


struct HttpRequestHeader
{
  enum class Method {GET, HEAD, POST, Unknown};
  Method method;
  std::string path;
  std::vector<std::string> headers;
  
  operator std::string();
};


struct HttpResponseHeader
{
  httpStatusCode statusCode;
  std::string statusText;
  std::vector<std::string> headers;
  
  operator std::string();
};


using HttpError = HttpResponseHeader;


std::string httpMethodToString(HttpRequestHeader::Method method);
HttpRequestHeader::Method httpMethodFromString(std::string& input);


const std::vector<char> specialCharacters = {':', '/', '?', '#', '[', ']', '@', '!', '$', '&', '\'', '(', ')', '*', '+', ',', ';', '='};
std::string urlDecode(std::string& input);

inline constexpr const char* protocolVersion = "HTTP/1.1";


} // namespace webserver

