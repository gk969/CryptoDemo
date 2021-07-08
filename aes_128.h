/*
 * FileName : AES.h
 *
 */
#ifndef __AES_H__
#define __AES_H__


class AES_128
{
public:
	void SetKey(unsigned char* key);
	unsigned char* Cipher(unsigned char* input, unsigned char* output);
	unsigned char* InvCipher(unsigned char* input, unsigned char* output);
	void* Cipher(void* input, void* output, int length = 0);
	void* InvCipher(void* input, void* output, int length);

private:
	unsigned char w[11][4][4];

	void KeyExpansion(unsigned char* key);
	unsigned char FFmul(unsigned char a, unsigned char b);

	void SubBytes(unsigned char state[][4]);
	void ShiftRows(unsigned char state[][4]);
	void MixColumns(unsigned char state[][4]);
	void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

	void InvSubBytes(unsigned char state[][4]);
	void InvShiftRows(unsigned char state[][4]);
	void InvMixColumns(unsigned char state[][4]);
};

#define ONLY_CFB
enum AESMode_t { MODE_OFB = 1, MODE_CFB, MODE_CBC, MODE_ECB };
class AES_128_mode {
private:
	AES_128 m_aes;
	AESMode_t	  m_mode;
public:
	AES_128_mode();
	void set_mode(AESMode_t _mode);
	void set_key(unsigned char* key);
	int  Encrypt(unsigned char* iv, unsigned char* input, int length, unsigned char* output);
	int  Decrypt(unsigned char* iv, unsigned char* input, int length, unsigned char* output);
};



#endif 