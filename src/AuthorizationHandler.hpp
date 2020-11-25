// (c) Marcel Kost 2020

#pragma once

#include "Http.hpp"
#include "ClientConnection.hpp"

#include <string>


namespace webserver {

struct AuthenticationTuple
{
  std::string username;
  std::string password;
};

class AuthorizationHandler
{
public:
  using RequestHandler = HttpResponseHeader (ClientConnection& clientConnection, HttpRequestHeader& requestHeader);

  AuthorizationHandler(const std::string realm, std::function<RequestHandler> requestHandler, AuthenticationTuple authenticationTuple)
   : requestHandler_(requestHandler), authenticationTuple_(authenticationTuple), realm_(realm) {};
  HttpResponseHeader handleRequest(ClientConnection& clientConnection, HttpRequestHeader& requestHeader);
  
private:
  std::function<RequestHandler> requestHandler_; 
  AuthenticationTuple authenticationTuple_;
  const std::string realm_;
};


} // namespace webserver

