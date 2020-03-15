#include "Http.hpp"


namespace webserver {


HttpRequestHeader::Method httpMethodFromString(std::string& input)
{
  if (input == "GET") return HttpRequestHeader::Method::GET;
  if (input == "HEAD") return HttpRequestHeader::Method::HEAD;
  if (input == "POST") return HttpRequestHeader::Method::POST;
  return HttpRequestHeader::Method::Unknown;
}


std::string httpMethodToString(HttpRequestHeader::Method method)
{
  if (method == HttpRequestHeader::Method::GET) return "GET";
  if (method == HttpRequestHeader::Method::HEAD) return "HEAD";;
  if (method == HttpRequestHeader::Method::POST) return "POST";;
  return "Unknown";
}


HttpRequestHeader::operator std::string()
{
  return httpMethodToString(method) + " " + path;
}


HttpResponseHeader::operator std::string()
{
  return std::to_string(statusCode) + " " + statusText;
}


} // namespace webserver

