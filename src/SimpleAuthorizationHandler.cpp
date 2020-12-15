// (c) Marcel Kost 2020

#include "SimpleAuthorizationHandler.hpp"
#include "Authorization.hpp"

#include <sstream>
#include <iostream>

namespace webserver {


HttpResponseHeader SimpleAuthorizationHandler::handleRequest(ClientConnection& clientConnection, HttpRequestHeader& requestHeader)
{
  if (!authorization::isAuthorizationProvided(requestHeader))
  {
    authorization::requestAuthorization(realm_);
  }

  auto receivedAuthenticationTuple = authorization::extractAuthenticationTuple(requestHeader);

  // check if provided data is valid
  if (receivedAuthenticationTuple.username != authenticationTuple_.username ||
      receivedAuthenticationTuple.password != authenticationTuple_.password)
  {
    authorization::requestAuthorization(realm_);
  }

  // authentication successful, forward request
  return requestHandler_(clientConnection, requestHeader);
}


} // namespace webserver
