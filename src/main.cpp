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
    
    while(1) {
      auto clientConnection = serverSocket.accept();
      std::cout << "accepted" << std::endl;
      
      try {
        auto requestHeader = clientConnection.readRequestHeader();
        
        if (requestHeader.method == webserver::HttpRequestHeader::Method::Unknown) {
          webserver::HttpResponseHeader errorHeader{501, "Not Implemented", {}};
          clientConnection.sendEmptyResponse(errorHeader);
        }
        
        std::cout << requestHeader.path << std::endl;
      } catch (webserver::HttpError& error) {
        clientConnection.sendEmptyResponse(error);
      } catch (webserver::ClientConnection::NetworkError& error) {
        std::cerr << error.what() << std::endl;
      }
    }
  } catch (webserver::ServerSocket::NetworkError& error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }
  
	return 0;
}

