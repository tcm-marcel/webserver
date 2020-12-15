// (c) Marcel Kost 2020

#pragma once

#include "Http.hpp"

#include <string>


namespace webserver {


namespace authorization
{

  struct AuthenticationTuple
  {
    std::string username;
    std::string password;
  };  

  inline bool isAuthorizationProvided(HttpRequestHeader& requestHeader)
  {
    return requestHeader.fields.contains("authorization");
  }

  AuthenticationTuple extractAuthenticationTuple(HttpRequestHeader& requestHeader);

  void requestAuthorization(std::string realm);

}


} // namespace webserver
