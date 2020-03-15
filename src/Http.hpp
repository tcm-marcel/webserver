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
};


struct HttpResponseHeader
{
  httpStatusCode statusCode;
  std::string statusText;
  std::vector<std::string> headers;
};


using HttpError = HttpResponseHeader;


HttpRequestHeader::Method httpMethodFromString(std::string& input);


} // namespace webserver

