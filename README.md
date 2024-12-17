### Disclaimer: not intended for production usage!
# base64-c
This project is intended as a C-Library for the conversion between binary data/text and Base64.

It will mainly be used in some other projects I plan to do to gather more experience in developing with C. Initially I intend to develop a password manager, that makes use of Base64 to store AES-256 encrypted passwords in a readable text file. In the long term I would like to implement a server that conforms to a subset of HTTP, therefore making base64 especially useful for Basic Authentication.

It exposes the following API via `base64.h`

```
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
```

# CLI
The project also provides a CLI tool for performing conversions directly via the command line. This CLI is entirely independent, allowing the library to be used on its own without relying on the CLI.

```
Usage: build/cli_base64 -e input | -d input
        -e input        Encode the specified input
        -d input        Decode the specified input
```

# Base64
Base64 is a binary-to-text encoding scheme that represents binary data in an ASCII string format. It is widely used for transmitting data over text-based protocols like email or embedding binary data in text formats such as JSON or XML.

It uses 64 printable characters to encode the data, specifically every 3 byte block is turned into a 4 byte block of printable characters. Additionally, a padding character `=` is used, for cases where the last block consists of less than 3 bytes. Logically, this conversion also leads to an increase in data size by ~33%.

Base64 alphabet:
`ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/`

## Algorithm
Since there are 64 possible characters, 64 different values can be encoded by one character, this is equal to 6 bits. However, since computers mainly work with multiples of 8 bytes, the algorithm needs to efficiently encode data. This works by taking each 3 byte block (= 24 bits), and then encoding it into the corresponding character sequence, each character encoding 6 bits (A = 000000, B = 000001, ...). In case less than 3 bytes are given as input, a padding character is added for the corresponding character, that would otherwise not be present.

For decoding this algorithm is reversed, and padding characters are ignored, in the sense, that no additional byte is parsed for them.

## Example conversions to Base64:
```
he => aGU= // padding character is appended
hel => aGVs // no padding necessary
hello => aGVsbG8=
```


# Todo
[X] Add documentation/comments (doxygen style)


# Ideas
[ ] Reading from stdin

[ ] Stream mode, that reads 3 bytes and directly prints the resulting 4 bytes for encoding, and vice-versa for decoding