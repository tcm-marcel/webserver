// (c) Marcel Kost 2020

#include "Util.hpp"

#include <algorithm>

// DEBUG
#include <iostream>
#include  <iomanip>

namespace webserver {


std::string urlDecode(const std::string& input)
{
  std::string output;
  output.reserve(input.size());
  
  for (auto it = input.begin(); it < input.end(); ++it) {
    char c = *it;
    if (c == '%') {
      char hex[3] = {*++it, *++it, '\0'};
      auto ord = std::strtoul(hex, 0, 16);
      output.push_back(static_cast<char>(ord));
    } else {
      output.push_back(c);
    }
  }
  
  return output;
}


constexpr inline base64::Byte base64::encodeByte(base64::Byte input)
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


constexpr inline base64::Byte base64::decodeByte(base64::Byte input)
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


static inline char extractCharacterFromBuffer(uint32_t& buffer)
{
  uint8_t byte = buffer & 0x0000003F;
  char character = base64::encodeByte(byte);
  buffer = buffer >> 6;
  return character;
}

static inline char skipCharacterFromBuffer(uint32_t& buffer)
{
  buffer = buffer >> 6;
  return '=';
}


std::string base64::encode(const std::string& input)
{
  std::string output;
  // TODO: output.reserve(std::ceil());
  
  for (auto it = input.begin(); it != input.end();)
  {
    uint32_t buffer = 0;
    char base64Characters[4];
    auto batch_size = std::min(input.end() - it, 3L);

    switch (batch_size)
    {
      case 3:
        reinterpret_cast<char*>(&buffer)[0] = *(it + 2);
      case 2:
        reinterpret_cast<char*>(&buffer)[1] = *(it + 1);
      case 1:
        reinterpret_cast<char*>(&buffer)[2] = *(it + 0);
    }

    if (batch_size == 3)
    {
      base64Characters[3] = extractCharacterFromBuffer(buffer);
      base64Characters[2] = extractCharacterFromBuffer(buffer);
      base64Characters[1] = extractCharacterFromBuffer(buffer);
      base64Characters[0] = extractCharacterFromBuffer(buffer);
    }
    else if (batch_size == 2)
    {
      base64Characters[3] = skipCharacterFromBuffer(buffer);
      base64Characters[2] = extractCharacterFromBuffer(buffer);
      base64Characters[1] = extractCharacterFromBuffer(buffer);
      base64Characters[0] = extractCharacterFromBuffer(buffer);
    }
    else
    {
      assert(batch_size == 1);
      base64Characters[3] = skipCharacterFromBuffer(buffer);
      base64Characters[2] = skipCharacterFromBuffer(buffer);
      base64Characters[1] = extractCharacterFromBuffer(buffer);
      base64Characters[0] = extractCharacterFromBuffer(buffer);
    }

    output.insert(output.end(), std::begin(base64Characters), std::end(base64Characters)); 

    it += batch_size;
  }

  return output;
}


static inline void addCharacterToBuffer(uint32_t& buffer, char character)
{
  char byte = (character == '=') ? 0 : base64::decodeByte(character);
  buffer = (buffer << 6) | byte;
}


std::string base64::decode(const std::string& input)
{
  std::string output;
  
  assert(input.size() % 4 == 0);

  for (auto it = input.begin(); it != input.end(); it += 4)
  {
    uint32_t buffer;

    char c = *(it + 2);
    char d = *(it + 3);

    addCharacterToBuffer(buffer, *(it + 0));
    addCharacterToBuffer(buffer, *(it + 1));
    addCharacterToBuffer(buffer, *(it + 2));
    addCharacterToBuffer(buffer, *(it + 3));
    
    if (c == '=')
    {
      assert(d == '=');
      output.push_back(reinterpret_cast<char*>(&buffer)[2]);
    }
    else if (d == '=')
    {
      output.push_back(reinterpret_cast<char*>(&buffer)[2]);
      output.push_back(reinterpret_cast<char*>(&buffer)[1]);
    }
    else
    {
      output.push_back(reinterpret_cast<char*>(&buffer)[2]);
      output.push_back(reinterpret_cast<char*>(&buffer)[1]);
      output.push_back(reinterpret_cast<char*>(&buffer)[0]);
    }
  }

  return output;
}


std::string toLowercase(std::string input)
{
  std::string output = input;
  std::transform(output.begin(), output.end(), output.begin(), 
    [] (unsigned char c) { return std::tolower(c); });
  return output;
}


} // namespace webserver

