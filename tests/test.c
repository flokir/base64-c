#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "base64.h"

#define MAX_LENGTH 255

char *generate_random_data(size_t size)
{
  if (size < 1)
  {
    return NULL;
  }

  if (size > MAX_LENGTH)
  {
    fprintf(stderr, "Exceeded max length");
    return NULL;
  }

  char *random_data = malloc(sizeof(char) * size); // allocate buffer for sequence and null terminator
  if (random_data == NULL)
  {
    return NULL;
  }

  for (int i = 0; i < size; i++)
  {
    unsigned char current_character = rand() % 256;
    random_data[i] = current_character;
  }

  return random_data;
}

// test that encoding and decoding give the correct results
int main(int argc, char **argv)
{
  char *word = "Hello";
  char *encoded_output = NULL;
  char *decoded_output = NULL;

  int encoded_count = base64_encode(word, strlen(word), &encoded_output);
  assert(encoded_count > 0);
  assert(encoded_output);

  int decoded_count = base64_decode(encoded_output, encoded_count, &decoded_output);
  assert(decoded_count > 0);
  assert(decoded_output);


  printf("%s\n", word);
  printf("%s\n", encoded_output);
  printf("%s\n", decoded_output);

  // assert that encoding and decoding returns the original word
  assert(strncmp(word, decoded_output, strlen(word)) == 0);
  assert(decoded_count == strlen(word));

  free(encoded_output);
  free(decoded_output);

  // srand(time(NULL));
  srand(42);

  printf("Running tests\n");
  for (int i = 0; i < 10000000; i++)
  {
    if (i % 100000 == 0)
    {
      printf("%d\n", i);
    }
    size_t size = (rand() % MAX_LENGTH) + 1;
    char *random_data = generate_random_data(size);
    char *encoded_output = NULL;
    char *decoded_output = NULL;

    int encoded_count = base64_encode(random_data, size, &encoded_output);
    
    assert(encoded_count > 0);
    assert(encoded_output);

    int decoded_count = base64_decode(encoded_output, encoded_count, &decoded_output);
    assert(decoded_count > 0);
    assert(decoded_output);

    //printf("%s\n", random_data);
    printf("%s\n", encoded_output);
    //printf("%s\n", decoded_output);

    // assert that encoding and decoding returns the original word
    assert(strncmp(random_data, decoded_output, size) == 0);
    assert(decoded_count == size);

    free(encoded_output);
    free(decoded_output);
    free(random_data);
  }
}