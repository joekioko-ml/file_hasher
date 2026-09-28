#ifndef HASH_H
#define HASH_H

#include <stddef.h>

#define SHA256_STRING_LENGTH 65

/*
 * Computes the SHA-256 hash of a file.
 *
 * Returns:
 *      0  -> Success
 *     -1  -> Error
 *
 * Output:
 *      output must be at least SHA256_STRING_LENGTH bytes.
 */

int hash_file(const char *filename,
              char output[SHA256_STRING_LENGTH]);

#endif
