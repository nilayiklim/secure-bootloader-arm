#ifndef ECDSA_H
#define ECDSA_H

#include <stdint.h>

#define ECDSA_PUB_KEY_SIZE 64
#define ECDSA_SIGNATURE_SIZE 64

int ecdsa_verify(const uint8_t *hash, const uint8_t *signature, const uint8_t *pub_key);

#endif 