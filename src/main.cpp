#include "ServerSocket.hpp"

#include <iostream>
#include <stdexcept>


int main(int argc, char* argv[])
{
  webserver::ServerSocket serverSocket;
  
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " PORT" << std::endl;
    return 1;
  }
  
  try {
    serverSocket.bindAndListen(std::stoi(argv[1]));
    
    while(true) {
      auto clientConnection = serverSocket.accept();
      
      try {
        auto requestHeader = clientConnection.readRequestHeader();
        
        if (requestHeader.method == webserver::HttpRequestHeader::Method::Unknown) {
          webserver::HttpResponseHeader errorHeader{501, "Not Implemented", {}};
          clientConnection.sendEmptyResponse(errorHeader);
        }
        
        webserver::HttpResponseHeader responseHeader;
        
        if (requestHeader.path == "/") {
          // Handle request
          responseHeader = webserver::HttpResponseHeader{200, "OK", {}};
          std::string content = "Hello World!\n";
          clientConnection.sendResponse(responseHeader, content);
        } else {
          responseHeader = webserver::HttpResponseHeader{404, "Not Found", {}};
          clientConnection.sendEmptyResponse(responseHeader);
        }
        
        // Print request and response to console
        std::cout << static_cast<std::string>(requestHeader) << " - " << static_cast<std::string>(responseHeader) << std::endl;
        
      } catch (webserver::HttpError& error) {
        std::cout << static_cast<std::string>(error) << std::endl;
        clientConnection.sendEmptyResponse(error);
      } catch (webserver::ClientConnection::NetworkError& error) {
        std::cout << error.what() << std::endl;
      }
    }
  } catch (webserver::ServerSocket::NetworkError& error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }
  
	return 0;
}

