// (c) Marcel Kost 2020

#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


namespace webserver {


using HttpStatusCode = unsigned int;
using HttpHeaderFields = std::unordered_map<std::string, std::string>;


struct HttpRequestHeader
{
  enum class Method {GET, HEAD, POST, Unknown};
  Method method;
  std::string path;
  HttpHeaderFields fields;
  
  operator std::string();
};


struct HttpResponseHeader
{
  HttpStatusCode statusCode;
  std::string statusText;
  HttpHeaderFields fields;
  
  operator std::string();
};


using HttpError = HttpResponseHeader;


std::string httpMethodToString(HttpRequestHeader::Method method);
HttpRequestHeader::Method httpMethodFromString(const std::string& input);


const std::vector<char> specialCharacters = {':', '/', '?', '#', '[', ']', '@', '!', '$', '&', '\'', '(', ')', '*', '+', ',', ';', '='};
std::string urlDecode(std::string& input);

inline constexpr const char* protocolVersion = "HTTP/1.1";


} // namespace webserver

