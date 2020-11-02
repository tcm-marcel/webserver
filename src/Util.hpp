// (c) Marcel Kost 2020

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>


namespace webserver {


const std::vector<char> specialCharacters = {':', '/', '?', '#', '[', ']', '@', '!', '$', '&', '\'', '(', ')', '*', '+', ',', ';', '='};
std::string urlDecode(const std::string& input);

namespace base64
{
  using Byte = uint8_t;

  class Base64InvalidCharacterException {};

  std::string encode(const std::string& input);
  std::string decode(const std::string& input);

  // private functions
  constexpr inline Byte encodeByte(Byte input);
  constexpr inline Byte decodeByte(Byte input);
} // namespace base64

std::string toLowercase(std::string input);

} // namespace webserver

