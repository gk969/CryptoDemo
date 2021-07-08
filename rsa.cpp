#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "heap.h"
#include "rsa.h"
#include "bignum.h"

uint8_t KEY_EXPONENT[3] = {0x01, 0x00, 0x01}; //65537
#define RAND_LEN        16
#define HEAP_BUF_SIZE   4096

static int public_block_operation(uint8_t* out, uint8_t* in, uint32_t in_len, uint8_t* modulus) {
    
    uint32_t edigits, ndigits;
    bn_t* c = (bn_t*)heap_alloc(BN_MAX_DIGITS * sizeof(bn_t));
    bn_t* e = (bn_t*)heap_alloc(BN_MAX_DIGITS * sizeof(bn_t));
    bn_t* m = (bn_t*)heap_alloc(BN_MAX_DIGITS * sizeof(bn_t));
    bn_t* n = (bn_t*)heap_alloc(BN_MAX_DIGITS * sizeof(bn_t));
    
    bn_decode(m, BN_MAX_DIGITS, in, in_len);
    bn_decode(n, BN_MAX_DIGITS, modulus, RSA_MAX_MODULUS_LEN);
    bn_decode(e, BN_MAX_DIGITS, KEY_EXPONENT, sizeof(KEY_EXPONENT));
    
    ndigits = bn_digits(n, BN_MAX_DIGITS);
    edigits = bn_digits(e, BN_MAX_DIGITS);
    
    if (bn_cmp(m, n, ndigits) >= 0) {
        heap_free(c);
        heap_free(e);
        heap_free(m);
        heap_free(n);
        return ERR_WRONG_DATA;
    }
    
    bn_mod_exp(c, m, e, edigits, n, ndigits);
    
    uint32_t out_len = (KEY_M_BITS + 7) / 8;
    bn_encode(out, out_len, c, ndigits);
    
    heap_free(c);
    heap_free(e);
    heap_free(m);
    heap_free(n);
    return 0;
}

int rsa_public_encrypt(uint8_t* pub_key, uint8_t* in, uint32_t in_len, uint8_t* rand, uint8_t* out, uint32_t* heap_buf) {
    heap_init(heap_buf, HEAP_BUF_SIZE);
    // alloc_test();
    
    int status;
    uint8_t pkcs_block[RSA_MAX_MODULUS_LEN];
    uint32_t i, modulus_len;
    
    modulus_len = (RSA_MAX_MODULUS_BITS + 7) / 8;
    if (in_len + 11 > modulus_len) {
        return ERR_WRONG_LEN;
    }
    
    pkcs_block[0] = 0;
    pkcs_block[1] = 2;
    for (i = 2; i < modulus_len - in_len - 1; i++) {
        pkcs_block[i] = rand[i % RAND_LEN];
    }
    
    pkcs_block[i++] = 0;
    
    memcpy((uint8_t*)&pkcs_block[i], (uint8_t*)in, in_len);
    status = public_block_operation(out, pkcs_block, modulus_len, pub_key);
    
    return status;
}

