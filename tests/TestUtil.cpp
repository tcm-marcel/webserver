#include "Util.hpp"

#include <iostream>
#include <string>


#ifdef NDEBUG
#error NDEBUG must not be set for testing
#endif

using namespace webserver;


void testBase64EncodeByte()
{
  assert (base64::encodeByte(13) == 'N');
  assert (base64::encodeByte(30) == 'e');
  assert (base64::encodeByte(59) == '7');
  assert (base64::encodeByte(62) == '+');
  assert (base64::encodeByte(63) == '/');
}


void testBase64DecodeByte()
{
  assert (base64::decodeByte('N') == 13);
  assert (base64::decodeByte('e') == 30);
  assert (base64::decodeByte('7') == 59);
  assert (base64::decodeByte('+') == 62);
  assert (base64::decodeByte('/') == 63);
}


void testBase64Encode()
{
  assert (base64::encode("Man") == std::string("TWFu"));
  assert (base64::encode("Ma") == std::string("TWE="));
  assert (base64::encode("M") == std::string("TQ=="));
}


void testBase64Decode()
{
  assert (base64::decode("TWFu") == std::string("Man"));
  assert (base64::decode("TWE=") == std::string("Ma"));
  assert (base64::decode("TQ==") == std::string("M"));
}


int main()
{
  testBase64EncodeByte();
  testBase64DecodeByte();
  testBase64Encode();
  testBase64Decode();

  std::cout << "Tests executed successfully. " << std::endl;
}
