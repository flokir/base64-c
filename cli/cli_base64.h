#pragma once

/*
 * Enum representing the operation mode for the program.
 * Possible values:
 * - ENCODE: Indicates that the program should encoded the provided text to base64.
 * - DECODE: Indicates that the program should decode the given base64 text.
 */
enum cli_mode
{
  ENCODE,
  DECODE
} typedef cli_mode_t;

/*
 * Struct used to store the current arguments of the cli tool.
 * Fields:
 * - current_mode: Current operation mode
 * - input: The text to be encoded/decoded
 */
struct cli_args
{
  cli_mode_t current_mode;
  char *input;
} typedef cli_args_t;