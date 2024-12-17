#pragma once

#include <stdlib.h>

/**
 * @brief Encodes a sequence of up to 3 characters into a 4-character Base64 sequence.
 * 
 * This function takes a sequence of up to 3 input characters and encodes them 
 * into a 4-character Base64 representation. The encoded output is written 
 * to the provided output buffer.
 * 
 * @param[in] to_encode Pointer to the input buffer containing the characters to encode.
 *                      Must point to at least `input_length` bytes.
 * @param[in] input_length The length of the input data in `to_encode`. 
 *                         Must be between 1 and 3, inclusive.
 * @param[out] output Pointer to the buffer where the 4-character Base64-encoded output 
 *                    will be stored. The buffer must be able to hold at least 4 characters.
 * 
 * @return The number of Base64 characters written to the output buffer (always 4 for valid inputs). Returns -1 if an error occured and 0 if input_length == 0.
 */
int base64_lookup_encode(char *to_encode, size_t input_length, char *output);

/**
 * @brief Decodes a 4-character Base64 sequence into the corresponding 3 original characters.
 * 
 * This function takes a 4-character Base64-encoded input sequence and decodes it into 
 * up to 3 bytes of original data. The decoded output is written to the provided output buffer.
 * 
 * @param[in] to_decode Pointer to the input buffer containing the Base64-encoded sequence.
 *                      Must point to at least `input_length` bytes.
 * @param[in] input_length The length of the input data in `to_decode`. 
 *                         Must be exactly 4 for valid Base64 sequences.
 * @param[out] output Pointer to the buffer where the decoded output will be stored. 
 *                    The buffer must have space for at least 3 bytes.
 * 
 * @return The number of characters written to the output buffer (between 1 and 3).
 *         Returns 0 if input_length == 0 and -1 if an error occured or an invalid character is contained.
 */
int base64_lookup_decode(char *to_decode, size_t input_length, char *output);
