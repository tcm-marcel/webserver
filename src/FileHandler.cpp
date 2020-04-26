// (c) Marcel Kost 2020

#include "FileHandler.hpp"


#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


namespace webserver {


HttpResponseHeader FileHandler::handleRequest(ClientConnection& clientConnection, HttpRequestHeader& requestHeader)
{
  validateRequestMethod(requestHeader);
  
  // Strip first slash from path for std::filesystem compatibility
  Path fullPath = basePath_ / (requestHeader.path.data() + 1);
  
  if (!fs::exists(fullPath) || !fs::canonical(fullPath).string().starts_with(basePath_.string())) {
    HttpError error{404, "Not Found", {}};
    clientConnection.sendEmptyResponse(error);
    return error;
  }
  
  HttpResponseHeader responseHeader{200, "OK", {}};
  
  // TODO: Handle HEAD method
  
  if (fs::is_regular_file(fullPath)) {
    // Send file
    
    int fd = open(fullPath.c_str(), O_RDONLY);
    
    // Add MIME type header
    auto mimeType = getMimeType(fullPath, fd);
    responseHeader.headers.push_back("Content-Type: " + mimeType);
    
    clientConnection.sendResponse(responseHeader, fd, fs::file_size(fullPath));
    
    close(fd);
    
  } else {
    // Send directory listing
    auto listing = generateDirListing(fullPath);
    responseHeader.headers.push_back("Content-Type: text/html");
    
    clientConnection.sendResponse(responseHeader, listing);
    
  }
  
  return responseHeader; 
}


void FileHandler::validateRequestMethod(HttpRequestHeader& requestHeader) const
{
  if (requestHeader.method != webserver::HttpRequestHeader::Method::GET && 
      requestHeader.method != webserver::HttpRequestHeader::Method::HEAD)
    throw HttpError{501, "Not Implemented", {}};
}


std::string FileHandler::getMimeType(Path& path, int fd)
{
  // Try some known magic numbers
  const size_t chunk_size = 64;
  char chunk[chunk_size];
  read(fd, chunk, chunk_size);
  lseek(fd, 0, SEEK_SET);
  
  const char pngMagicNumber[] = "\211PNG\r\n\032\n";
  const char jpgMagicNumber[] =  "\0FF\0D8\0FF";
  
  if (std::memcmp(chunk, pngMagicNumber, sizeof(pngMagicNumber)) == 0)
    return "image/png";
    if (std::memcmp(chunk, jpgMagicNumber, sizeof(jpgMagicNumber)) == 0)
    return "image/jpeg";
  
  // Fall back to file name extension
  auto extension = path.extension().string();
  std::transform(extension.begin(), extension.end(),extension.begin(), ::tolower); 
  if (extension == ".html" || extension == ".htm")
    return "text/html";
  if (extension == ".png")
    return "image/png";
  if (extension == ".jpg" || extension == ".jpeg")
    return "image/jpeg";
  else
    return "text/plain";
}

std::string FileHandler::generateDirListing(Path& path)
{
  std::string html = "<html><head><title>Directory Listing</title></head><body><h1>" + stripBasePath(fs::canonical(path)) + "/</h1>";
  html += "<ul>";
  
  if (path != basePath_)
    html += "<li><a href=\"/\">..</a></li>";

  // TODO: urlEncode
  std::vector<Path> children;
  std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(children));
  std::sort(children.begin(), children.end());

  for (auto& child : children) {
    auto relative = stripBasePath(child);
    
    html += "<li><a href=\"" + relative + "\">" + relative + "</a></li>";
  }
  
  html += "</ul></body></html>";
  
  return html;
}


std::string FileHandler::stripBasePath(Path path)
{
  return path.string().substr(basePath_.string().size());
}


} // namespace webserver

