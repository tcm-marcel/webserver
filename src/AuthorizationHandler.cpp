// (c) Marcel Kost 2020

#include "AuthorizationHandler.hpp"
#include "Util.hpp"

#include <sstream>
#include <iostream>

namespace webserver {


HttpResponseHeader AuthorizationHandler::handleRequest(ClientConnection& clientConnection, HttpRequestHeader& requestHeader)
{
  HttpResponseHeader responseHeader;
  
  if (requestHeader.fields.contains("authorization"))
  {
    std::istringstream value_stream(requestHeader.fields["authorization"]);
    std::string authenticationType, credentialsEncoded;
    AuthenticationTuple receivedAuthenticationTuple;

    value_stream >> authenticationType >> credentialsEncoded;
    
    // fail on unsupported type
    if (toLowercase(authenticationType) != "basic")
      throw HttpError{501, "Not Implemented", {}};

    // decode credentials
    auto credentialsDecodedStream = std::istringstream(base64::decode(credentialsEncoded));

    /// DEBUG
    std::cerr << credentialsEncoded << " --> " << base64::decode(credentialsEncoded) << std::endl;
    
    std::getline(credentialsDecodedStream, receivedAuthenticationTuple.username, ':');
    std::getline(credentialsDecodedStream, receivedAuthenticationTuple.password);

    // check if provided data is valid
    if (receivedAuthenticationTuple.username == authenticationTuple_.username && 
        receivedAuthenticationTuple.password == authenticationTuple_.password)
    {
      // authentication successful, forward request
      return requestHandler_(clientConnection, requestHeader);
    }
    else
    {
      // authentication failed
      throw HttpError{401, "Unauthorized", {
        {"WWW-Authenticate", "Basic realm=\"" + realm_ + "\""},
      }};
    }
    
  }
  else
  {
    // request Authorization
    throw HttpError{401, "Unauthorized", {
      {"WWW-Authenticate", "Basic realm=\"" + realm_ + "\""},
    }};
  }
  
  return responseHeader; 
}





} // namespace webserver
