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
  constexpr Byte encodeByte(Byte input)
  {
    assert(static_cast<uint8_t>(input) < 64);

    if (input < 26)
      return 'A' + input;
    else if (26 <= input && input < 52)
      return 'a' + input - 26;
    else if (52 <= input && input < 62)
      return '0' + input - 52;
    else if (input == 62)
      return '+';
    else // input_balue == 63
      return '/';
  }

  constexpr Byte decodeByte(Byte input)
  {
    if ('A' <= input && input <= 'Z')
      return input - 'A';
    else if ('a' <= input && input <= 'z')
      return 26 + input - 'a';
    else if ('0' <= input && input <= '9')
      return 52 + input - '0';
    else if (input == '+')
      return 62;
    else if (input == '/')
      return 63;
    else // invalid character
      throw base64::Base64InvalidCharacterException();
  }
} // namespace base64

std::string toLowercase(std::string input);

} // namespace webserver

