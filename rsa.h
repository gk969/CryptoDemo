/*****************************************************************************
Filename    : rsa.h
Author      : Terrantsh (tanshanhe@foxmail.com)
Date        : 2018-9-20 11:22:22
Description : RSA加密头文件
*****************************************************************************/
#ifndef __RSA_H__
#define __RSA_H__

#include <stdint.h>

extern uint8_t KEY_EXPONENT[3];

#define KEY_M_BITS      2048

// RSA key lengths
#define RSA_MAX_MODULUS_BITS    KEY_M_BITS
#define RSA_MAX_MODULUS_LEN     ((RSA_MAX_MODULUS_BITS + 7) / 8)
#define RSA_MAX_PRIME_BITS      ((RSA_MAX_MODULUS_BITS + 1) / 2)
#define RSA_MAX_PRIME_LEN       ((RSA_MAX_PRIME_BITS + 7) / 8)

// Error codes
#define ERR_WRONG_DATA                      0x1001
#define ERR_WRONG_LEN                       0x1002

typedef uint64_t dbn_t;
typedef uint32_t bn_t;


int rsa_public_encrypt(uint8_t* pub_key, uint8_t* in, uint32_t in_len, uint8_t* rand, uint8_t* out, uint32_t* heap_buf);


#endif  // __RSA_H__
