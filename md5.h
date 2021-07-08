#ifndef MD5_H
#define MD5_H

#define MD5_SIZE    16

unsigned char* MD5_Compute(unsigned char* md5_buf, unsigned char* data, unsigned int len);
 
#endif