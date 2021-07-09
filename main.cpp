/*****************************************************************************
Filename    : main.c
Author      : Terrantsh (tanshanhe@foxmail.com)
Date        : 2018-9-22 18:18:54
Description : 实现了RSA2048加密解密的各项功能，并能够进行最大256位的加密操作
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "rsa.h"
#include "aes_128.h"
#include "md5.h"
#include "utils.h"
#include "heap.h"

uint8_t pub_key[] = {
    0xB0, 0x1D, 0x83, 0x85, 0x23, 0x65, 0x20, 0xC6,
    0xBD, 0x10, 0xE6, 0x38, 0xEF, 0x74, 0xF5, 0xF3,
    0x6C, 0xD1, 0xD1, 0x07, 0x2F, 0x3E, 0xD8, 0xAA,
    0x9C, 0xE2, 0xC6, 0x22, 0x32, 0x6E, 0x5B, 0x58,
    0xFF, 0x7E, 0x2B, 0x97, 0xE8, 0x2F, 0xE9, 0xD9,
    0x88, 0x3F, 0xD5, 0x0B, 0x9E, 0x36, 0x6F, 0xAF,
    0x47, 0x0C, 0xD8, 0xFC, 0x80, 0xB6, 0x58, 0x78,
    0x4E, 0x0C, 0x9C, 0x96, 0x81, 0xFC, 0xFA, 0x5D,
    0x06, 0x0A, 0xEA, 0xA5, 0x93, 0x25, 0x0B, 0x02,
    0x9C, 0x63, 0xFC, 0xB7, 0xA5, 0x49, 0xB0, 0xDB,
    0x9F, 0x37, 0x10, 0x97, 0x01, 0xBE, 0xAD, 0xFB,
    0x3F, 0x6D, 0x99, 0xC4, 0xCB, 0x62, 0x6D, 0x88,
    0x9B, 0x7C, 0xE2, 0xEB, 0x24, 0x20, 0x06, 0xA8,
    0x2B, 0x04, 0xA1, 0xB9, 0xCE, 0x5F, 0x20, 0xF7,
    0x0C, 0x57, 0x6F, 0x34, 0xF4, 0xFC, 0xF0, 0xB2,
    0x95, 0x5A, 0x1F, 0x81, 0x5D, 0xA9, 0x54, 0x7B,
    0x7F, 0x82, 0x67, 0x34, 0x5D, 0xD0, 0x7C, 0xC7,
    0x67, 0xEF, 0x58, 0x4F, 0x2E, 0x5C, 0x42, 0x2A,
    0xDB, 0xD6, 0x8B, 0xB6, 0x2D, 0xF7, 0x87, 0x33,
    0x6B, 0xE4, 0x84, 0x19, 0xA2, 0x76, 0x8C, 0xC9,
    0x64, 0x76, 0x1F, 0xE9, 0x78, 0xF3, 0xAA, 0x6C,
    0x5A, 0xC5, 0x6D, 0x9E, 0x05, 0x40, 0xE0, 0x9D,
    0x67, 0x7C, 0xC8, 0x10, 0x07, 0x31, 0x53, 0xA7,
    0xC8, 0xDF, 0xB6, 0xA4, 0xFB, 0xF2, 0x6C, 0x39,
    0xDB, 0x0A, 0xB6, 0x91, 0x42, 0x52, 0x26, 0x53,
    0xFB, 0xD6, 0x37, 0xE7, 0xE9, 0x23, 0xA2, 0xCD,
    0xB1, 0x38, 0x2B, 0x49, 0x05, 0xA1, 0x66, 0x85,
    0x52, 0x96, 0xD3, 0xA2, 0xCD, 0x01, 0x85, 0x87,
    0xE4, 0xEE, 0x8F, 0xFB, 0xF0, 0xA6, 0x67, 0x75,
    0x1E, 0xB8, 0x32, 0x71, 0xAA, 0x36, 0x40, 0x4F,
    0x9F, 0xFD, 0xDB, 0x90, 0x10, 0xC4, 0x84, 0xB8,
    0x59, 0x9D, 0xEE, 0xB0, 0x46, 0x85, 0x57, 0xE7
};


/*
 * RSA2048 encrypt and decrypt
 * include rsa.c/bignum.c/rsa.h/bignum.h/keys.h
 */
static int RSA2048(void) {
    uint8_t output[256];
    
    // message to encrypt
    uint8_t input[] = "SSSQQQ789abcdef";
    int inputLen = strlen((const char*)input);
    
    uint8_t rand[16];
    uint32_t heap_buf[4096/sizeof(uint32_t)];
    
    rsa_public_encrypt(pub_key, input, inputLen, rand, output, heap_buf);
    log_ln("rsa_public_encrypt");
    log_ln("modulus ");
    log_hex(pub_key, sizeof(pub_key));
    log("exponent ");
    log_hex(KEY_EXPONENT, sizeof(KEY_EXPONENT));
    log_ln("input %s", input);
    log_ln("output len %d", RSA_MAX_MODULUS_LEN);
    for (int i = 0; i < RSA_MAX_MODULUS_LEN; i++) {
        log("%02X", output[i]);
    }
    log_ln("");
    
    return 0;
}

void test_aes(){
    unsigned char plain[] = "0123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef0123456780123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef0123456780123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef0123456780123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef0123456780123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef0123456780123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef0123456780123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef0123456780123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef01234567890123456789abcdef0123456789abcdef0123456789abcdef012345678"; 
    unsigned char key[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    unsigned char iv[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };

    AES_128_cfb aes;
    aes.set_key(key);

    int out_len = aes.Encrypt(iv, plain, sizeof(plain), plain);
    log_ln("Encrypt %d", out_len);
    log_hex(plain, out_len);

    out_len = aes.Decrypt(iv, plain, sizeof(plain), plain);
    log_ln("Decrypt %d", out_len);
    log_ln("%s", plain);
}

void test_md5() {
    unsigned char plain[] = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789";
    unsigned char md5[MD5_SIZE];
    MD5_Compute(md5, plain, 58);

    log_ln("md5");
    log_hex(md5, MD5_SIZE);
}

int main(int argc, char const* argv[]) {
    clock_t start, finish;
    double  duration;
    start = clock();    // init start time
    // RSA2048();
    test_aes();
    // test_md5();
    finish = clock();   // print end time
    duration = (double)(finish - start) / CLOCKS_PER_SEC;   // print encrypt and decrypt time
    printf("%f seconds\n", duration);
    return 0;
}
