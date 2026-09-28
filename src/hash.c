#include "hash.h"

#include <stdio.h>
#include <string.h>

#include <openssl/sha.h>

#define BUFFER_SIZE 65536

int hash_file(const char *filename,
              char output[SHA256_STRING_LENGTH])
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
        return -1;

    SHA256_CTX ctx;

    SHA256_Init(&ctx);

    unsigned char buffer[BUFFER_SIZE];

    size_t bytes_read;

    while ((bytes_read =
            fread(buffer,
                  1,
                  BUFFER_SIZE,
                  fp)) > 0)
    {
        SHA256_Update(&ctx,
                      buffer,
                      bytes_read);
    }

    fclose(fp);

    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_Final(digest,
                 &ctx);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(output + (i * 2),
                "%02x",
                digest[i]);
    }

    output[64] = '\0';

    return 0;
}
