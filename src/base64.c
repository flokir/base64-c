#include "base64.h"
#include "base64_lookup.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_SIZE 65536

// char *data, size_t size
// returns a \0 terminated string, corresponding to the base64 encoded data
int base64_encode(char *data, size_t length, char **output)
{
  if (length == 0) {
    return 0;
  }
  
  // since 3 bytes are encoded into 4 bytes, we need to allocate a buffer of the corresponding size
  // however it is actually also possible that 0 is contained in our data? -> e.g. an int with value 0
  // therefore strlen is not a valid measure, instead the length should be passed to the method

  // add padding to our text
  // compute the new length, also considering possible padding
  size_t encoded_length = ((length + 2) / 3) * 4;
  size_t encoded_length_with_terminator = encoded_length + 1;

  // create buffer with the size of our base64 string and the \0 terminator
  size_t output_size = encoded_length_with_terminator;
  char *base64_output = (char *)malloc(output_size);
  if (!base64_output)
  {
    fprintf(stderr, "Unable to allocate memory\n");
    return -1;
  }

  memset(base64_output, 0, output_size);

  // Each 3 byte/character slice is encoded into 4 ASCII characters and appended to the encoded base64 buffer

  // Keep track of current slice and current writing position in output
  char *current_slice = NULL;
  char *current_output = base64_output;
  // Iterate through each 3 character slice
  for (size_t index = 0; index < length; index += 3)
  {
    // begin the current slice at the index offset
    current_slice = data + index;
    // compute how many characters are remaining (characters at index, index + 1, ...)
    size_t remaining = length - index;

    // encode the characters and append
    // update the current output pointer

      size_t inputSize = remaining;
      if (inputSize > 3)
      {
        inputSize = 3;
      }
      // increment current output by the number of added characters
      int added_characters = base64_lookup_encode(current_slice, inputSize, current_output);
      if (added_characters < 0) {
        free(base64_output);
        return -1;
      }
      current_output += added_characters;
    }

  // null terminate the string
  base64_output[encoded_length] = '\0';

  // set the output pointer as our current buffer
  *output = base64_output;
  // return the strlen of our output buffer
  // therefore not considering the \0 terminator
  return encoded_length;
}

int base64_decode(char *base64, size_t length, char **output)
{
  if (length == 0) {
    return 0;
  }
  // compute the maximum possible length of the decoded output
  size_t decoded_length = length * 3 / 4 + 1;

  char *decoded_output = (char *)malloc(decoded_length);
  if (decoded_output == NULL)
  {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  memset(decoded_output, 0, decoded_length);

  // Keep track of current slice and current writing position in output
  char *current_slice = NULL;
  char *current_output = decoded_output;

  // Each 4 character slice is decoded into the corresponding 3 bytes
  for (int index = 0; index < length; index += 4)
  {
    current_slice = base64 + index;
    size_t remaining = length - index;
    size_t inputSize = remaining;
    if (inputSize > 4)
    {
      inputSize = 4;
    }

    // increment current output by the number of added characters
    int added_characters = base64_lookup_decode(current_slice, inputSize, current_output);
    if (added_characters < 0) {
      free(decoded_output);
      return -1;
    }
    current_output += added_characters;
  }

  // consider padding

  // based on current_output we need to trim our output to remove the characters,
  // that correspond to padding in the base64 string
  int new_length = current_output - decoded_output;

  // reallocate our buffer to incorporate the reduced size
  *output = realloc(decoded_output, new_length);
  return new_length;
}