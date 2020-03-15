#include "Http.hpp"


namespace webserver {


HttpRequestHeader::Method httpMethodFromString(std::string& input)
{
  if (input == "GET") return HttpRequestHeader::Method::GET;
  if (input == "HEAD") return HttpRequestHeader::Method::HEAD;
  if (input == "POST") return HttpRequestHeader::Method::POST;
  return HttpRequestHeader::Method::Unknown;
}


} // namespace webserver

