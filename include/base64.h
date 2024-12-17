#pragma once

#include <stddef.h>

/**
 * @brief Encodes a given text or binary data into its Base64 representation.
 *
 * This function takes an input buffer containing data (text or binary) and encodes it
 * into a Base64-encoded string. The encoded string is returned as a null-terminated
 * string via the `output` pointer.
 *
 * @param[in] data Pointer to the input buffer containing the data to encode.
 *                 The data can be either text or binary.
 * @param[in] length The length of the input data in bytes.
 * @param[out] output Pointer to a variable where the Base64-encoded output will be stored.
 *                    This should be a valid pointer to a `char *` variable that will hold
 *                    the null-terminated Base64 string.
 *
 * @return The number of characters in the encoded Base64 string (not including the null terminator).
 *
 * @note The `output` buffer is allocated dynamically inside this function, and the caller
 *       is responsible for freeing the allocated memory after use.
 */
int base64_encode(char *data, size_t length, char **output);

/**
 * @brief Decodes a Base64-encoded string into the corresponding binary data or text.
 * 
 * This function takes a Base64-encoded string and decodes it back into the original 
 * binary data or text. The decoded data is stored in the provided `output` buffer. 
 * The function does not append a null terminator to the decoded output, since the input
 * could represent binary data (which may not have a null terminator).
 * 
 * @param[in] base64 Pointer to the Base64-encoded string to decode. 
 *                   This string must be a valid Base64-encoded data.
 * @param[in] length The length of the Base64-encoded data in the `base64` string.
 * @param[out] output Pointer to a variable where the decoded output will be stored. 
 *                    This will point to a buffer containing the decoded binary data/text.
 * 
 * @return The size of the decoded data (in bytes). If the function fails, it returns -1.
 *         The size does not include any padding that may be present in the input Base64 string.
 * 
 * @note The caller is responsible for freeing the memory allocated for the `output` buffer 
 *       after usage.
 * 
 * @warning The `base64` string must be a valid Base64-encoded string. If the input is 
 *          invalid (e.g., contains characters not part of the Base64 alphabet), 
 *          the function will return -1.
 */
int base64_decode(char *base64, size_t length, char **output);