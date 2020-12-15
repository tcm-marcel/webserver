// (c) Marcel Kost 2020

#pragma once

#include "Http.hpp"
#include "ClientConnection.hpp"
#include "Authorization.hpp"

#include <string>


namespace webserver {


class SimpleAuthorizationHandler
{
public:
  using RequestHandler = HttpResponseHeader (ClientConnection& clientConnection, HttpRequestHeader& requestHeader);

  SimpleAuthorizationHandler(const std::string realm, std::function<RequestHandler> requestHandler, authorization::AuthenticationTuple authenticationTuple)
   : requestHandler_(requestHandler), authenticationTuple_(authenticationTuple), realm_(realm) {};
  HttpResponseHeader handleRequest(ClientConnection& clientConnection, HttpRequestHeader& requestHeader);
  
private:
  std::function<RequestHandler> requestHandler_; 
  authorization::AuthenticationTuple authenticationTuple_;
  const std::string realm_;
};


} // namespace webserver

