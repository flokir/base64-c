#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <bits/getopt_core.h>

#include "cli_base64.h"
#include "base64.h"

char *executable_name = NULL;

/*
 * Print the correct usage of this tool.
 */
void print_usage(void)
{
  fprintf(stderr, "Usage: %s -e input | -d input\n\t-e input\tEncode the specified input\n\t-d input\tDecode the specified input", executable_name);
}

/*
 * Extract the mode and input from the cli and store it in the cli_args_t struct.
 */
cli_args_t parse_args(int argc, char **argv)
{
  int opt = 0;
  cli_args_t cli_args = {0};

  bool encode = false;
  bool decode = false;
  while ((opt = getopt(argc, argv, "ed")) != -1)
  {
    switch (opt)
    {
    case 'e':
      encode = true;
      break;
    case 'd':
      decode = true;
      break;
    default:
      print_usage();
      exit(EXIT_FAILURE);
    }
  }

  // both options were specified
  // however either encode or decode is supported
  if (encode && decode)
  {
    print_usage();
    exit(EXIT_FAILURE);
  }

  // we still have a positional argument left
  if (optind < argc)
  {
    cli_args.input = argv[optind];
    if (encode)
    {
      cli_args.current_mode = ENCODE;
    }
    if (decode)
    {
      cli_args.current_mode = DECODE;
    }
    return cli_args;
  }

  // no positional argument present
  print_usage();
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
  executable_name = argv[0];
  cli_args_t args = parse_args(argc, argv);
  // Print the current options for now
  printf("%d %s\n", args.current_mode, args.input);

  if (args.current_mode == ENCODE)
  {
    // encode the input
    char *encoded = NULL;
    int encoded_size = base64_encode(args.input, strlen(args.input), &encoded);

    if (encoded_size == 0) {
      fprintf(stderr, "No data passed to encode\n");
      exit(EXIT_FAILURE);
    }
    if (encoded_size < 0) {
      fprintf(stderr, "Error during encoding\n");
      exit(EXIT_SUCCESS);
    }

    printf("%s\n", encoded);

    // Free the memory allocated for the base64 string
    free(encoded);

    exit(EXIT_SUCCESS);
  }

  if (args.current_mode == DECODE) {
    char *decoded = NULL;
    int decoded_size = base64_decode(args.input, strlen(args.input), &decoded);

    if (decoded_size == 0) {
      fprintf(stderr, "No data passed to decode\n");
      exit(EXIT_FAILURE);
    }
    if (decoded_size < 0) {
      fprintf(stderr, "Error during decoding\n");
      exit(EXIT_SUCCESS);
    }
    
    // append a null terminator for printing to our decoded data/text
    char *decoded_terminated = malloc(decoded_size + 1);
    if (!decoded_terminated) {
      fprintf(stderr, "Couldn't allocate memory\n");
      exit(EXIT_FAILURE);
    }
    memset(decoded_terminated, 0, decoded_size + 1);
    strncpy(decoded_terminated, decoded, decoded_size);
    decoded_terminated[decoded_size] = '\0';

    printf("%s\n", decoded_terminated);

    free(decoded_terminated);
    free(decoded);

    exit(EXIT_SUCCESS);
  }

  print_usage();
  exit(EXIT_FAILURE);
}