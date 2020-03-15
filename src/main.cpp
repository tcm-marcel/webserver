#include "ServerSocket.hpp"
#include "FileHandler.hpp"

#include <iostream>
#include <stdexcept>


int main(int argc, char* argv[])
{ 
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " PORT BASE_PATH" << std::endl;
    return 1;
  }
  
  webserver::ServerSocket serverSocket;
  webserver::FileHandler fileHandler(argv[2]);
  
  try {
    serverSocket.bindAndListen(std::stoi(argv[1]));
    
    while(true) {
      auto clientConnection = serverSocket.accept();
      
      try {
        auto requestHeader = clientConnection.readRequestHeader();
        
        // Handle request
        webserver::HttpResponseHeader responseHeader = fileHandler.handleRequest(clientConnection, requestHeader);
        
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

