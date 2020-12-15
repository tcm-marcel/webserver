// (c) Marcel Kost 2020

#include "ServerSocket.hpp"
#include "FileHandler.hpp"
#include "SimpleAuthorizationHandler.hpp"

#include <future>
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

  std::function<webserver::SimpleAuthorizationHandler::RequestHandler> requestHandler = std::bind(&webserver::FileHandler::handleRequest, fileHandler, std::placeholders::_1, std::placeholders::_2);
  webserver::SimpleAuthorizationHandler authorizationHandler("Password required", requestHandler, {"username", "password"});
  
  try {
    serverSocket.bindAndListen(std::stoi(argv[1]));
    
    while(true) {
      auto clientConnection = serverSocket.accept();

      auto ignore = std::async(std::launch::async | std::launch::deferred, [&authorizationHandler] (webserver::ClientConnection clientConnection) {
        try {
          auto requestHeader = clientConnection.readRequestHeader();

          // Handle request
          //webserver::HttpResponseHeader responseHeader = fileHandler.handleRequest(clientConnection, requestHeader);
          webserver::HttpResponseHeader responseHeader = authorizationHandler.handleRequest(clientConnection, requestHeader);

          // Print request and response to console
          std::cout << static_cast<std::string>(requestHeader) << " - " << static_cast<std::string>(responseHeader) << std::endl;

        } catch (webserver::HttpError& error) {
          std::cout << static_cast<std::string>(error) << std::endl;
          clientConnection.sendEmptyResponse(error);

        } catch (webserver::ClientConnection::NetworkError& error) {
          std::cout << error.what() << std::endl;

        }
      }, clientConnection);
    }
  } catch (webserver::ServerSocket::NetworkError& error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }
  
	return 0;
}

