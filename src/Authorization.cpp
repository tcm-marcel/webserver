// (c) Marcel Kost 2020

#include "Authorization.hpp"
#include "Util.hpp"

#include <sstream>
#include <iostream>


namespace webserver {


authorization::AuthenticationTuple authorization::extractAuthenticationTuple(HttpRequestHeader& requestHeader)
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
  
  std::getline(credentialsDecodedStream, receivedAuthenticationTuple.username, ':');
  std::getline(credentialsDecodedStream, receivedAuthenticationTuple.password);

  return receivedAuthenticationTuple;
}

void authorization::requestAuthorization(std::string realm)
{
  // request Authorization
  throw HttpError{401, "Unauthorized", {
    {"WWW-Authenticate", "Basic realm=\"" + realm + "\""},
  }};
}


} // namespace webserver
