// (c) Marcel Kost 2020

#pragma once

#include "Http.hpp"
#include "ClientConnection.hpp"

#include <filesystem>
#include <stdexcept>
#include <string>


namespace fs = std::filesystem;


namespace webserver {


class FileHandler
{
public:
  using Path = fs::path;
  
  FileHandler(Path basePath)
   : basePath_(fs::canonical(basePath))
  {
    if (!fs::exists(basePath))
      throw std::runtime_error("Given base path doesn't exist: " + basePath.string());
  }

  HttpResponseHeader handleRequest(ClientConnection& clientConnection, HttpRequestHeader& requestHeader);
  
private:
  Path basePath_;
  
  void validateRequestMethod(HttpRequestHeader& requestHeader) const;
  std::string getMimeType(Path& path, int fd);
  std::string generateDirListing(Path& path);
  
  std::string stripBasePath(Path path);
};


} // namespace webserver

