// (c) Marcel Kost 2020

#include "Http.hpp"


namespace webserver {


HttpRequestHeader::Method httpMethodFromString(const std::string& input)
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


std::string urlDecode(std::string& input)
{
  std::string output;
  output.reserve(input.size());
  
  for (auto it = input.begin(); it < input.end(); ++it) {
    char c = *it;
    if (c == '%') {
      char hex[3] = {*++it, *++it, '\0'};
      auto ord = std::strtoul(hex, 0, 16);
      output.push_back(static_cast<char>(ord));
    } else {
      output.push_back(c);
    }
  }
  
  return output;
}


} // namespace webserver

