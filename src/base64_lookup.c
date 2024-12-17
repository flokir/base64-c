#include "base64_lookup.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

const char encode_lookup_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_lookup_encode(char *to_encode, size_t input_length, char *output)
{
  if (input_length < 1)
  {
    // return 0 to indicate that no new character was added
    return 0;
  }
  if (input_length > 3)
  {
    input_length = 3;
  }

  // construct an integer containing the up to 24bits of the input
  uint32_t bits = 0;

  for (int i = 0; i < input_length; i++)
  {
    // shift first character 16 bits to the left
    // shift second character 8 bits to the left
    // third character stays unshifted
    bits |= ((unsigned char)to_encode[i]) << (16 - (8 * i));
  }

  // determine the 4 output characters
  output[0] = encode_lookup_table[(bits >> 18) & 0x3F];
  output[1] = encode_lookup_table[(bits >> 12) & 0x3F];

  if (input_length > 1)
  {
    output[2] = encode_lookup_table[(bits >> 6) & 0x3F];
  }
  else
  {
    output[2] = '=';
  }
  if (input_length > 2)
  {
    output[3] = encode_lookup_table[bits & 0x3F];
  }
  else
  {
    output[3] = '=';
  }

  // always return 4, because padding is appended anyway
  return 4;
}

int base64_decode_value(char c)
{
  if (c >= 'A' && c <= 'Z')
  {
    return c - 'A'; // Values 0 to 25 for 'A' to 'Z'
  }
  if (c >= 'a' && c <= 'z')
  {
    return c - 'a' + 26; // Values 26 to 51 for 'a' to 'z'
  }
  if (c >= '0' && c <= '9')
  {
    return c - '0' + 52; // Values 52 to 61 for '0' to '9'
  }
  if (c == '+')
  {
    return 62; // Value 62 for '+'
  }
  if (c == '/')
  {
    return 63; // Value 63 for '/'
  }
  return -1; // Return -1 for any other character
}

int base64_lookup_decode(char *to_decode, size_t input_size, char *output)
{

  // printf("Lookup decode\n");
  if (input_size < 1)
  {
    return 0;
  }
  if (input_size > 4)
  {
    input_size = 4;
  }

  // construct integer containing the corresponding bits of the base64 characters
  uint32_t bits = 0;
  int paddingCount = 0;
  for (int i = 0; i < input_size; i++)
  {
    // obtain the corresponding value of the base64 character
    char currentCharacter = to_decode[i];

    if (currentCharacter == '=')
    {
      // padding character received
      // no more consecutive characters (except for padding)
      paddingCount++;
      bits = bits << 6;
      continue;
    }

    int lookupValue = base64_decode_value(currentCharacter);

    if (lookupValue < 0)
    {
      // not a valid character
      // printf("%d\n", (int) currentCharacter);
      printf("Invalid character\n");
      // ignore the character and instead use 0 as value
      // todo, or should this fail and return -1?
      // decided -> fail with invalid character
      return -1;
      // continue;
    }

    bits = (bits << 6) | lookupValue;
  }

  // todo -> this isn't even needed I guess?
  // since it is all set to 0 anyway?
  // Decode based on the padding count
  if (paddingCount == 0)
  {
    // No padding, all 3 bytes are valid
    output[0] = (bits >> 16) & 0xFF;
    output[1] = (bits >> 8) & 0xFF;
    output[2] = bits & 0xFF;
    return 3;
  }
  else if (paddingCount == 1)
  {
    // One padding, decode only the first 2 bytes
    output[0] = (bits >> 16) & 0xFF;
    output[1] = (bits >> 8) & 0xFF;
    return 2;
  }
  else if (paddingCount == 2)
  {
    // Two padding, decode only the first byte
    output[0] = (bits >> 16) & 0xFF;
    return 1;
  }

  fprintf(stderr, "Received invalid number of padding characters\n");
  return -1;
}