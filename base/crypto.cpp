/*
 * crypto.cpp
 *
 *  Created on: 2015-1-8
 *      Author: echo
 */
#include <string.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/des.h>

#include "crypto.h"

bool base64_encode(void* output, uint32_t& output_len, const void* input, uint32_t input_len)
{
	//prepare
	if(output_len<((double)input_len)*4.0/3.0+1.0)
		return false;
	memset(output, 0, output_len);
	//encode
	BIO * b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	BIO * bmem = BIO_new(BIO_s_mem());
	BUF_MEM buf_mem;
	buf_mem.data = (char*)output;
	buf_mem.max = output_len;
	buf_mem.length = 0;
	BIO_set_mem_buf(bmem, &buf_mem, BIO_NOCLOSE);
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, input_len);
	int ret = BIO_flush(b64);
	if(ret<=0)
		return false;
	output_len = buf_mem.length;
	//free resource
	BIO_free_all(b64);
	return true;
}
bool base64_decode(void* output, uint32_t& output_len, const void* input, uint32_t input_len)
{
	//prepare
	if(output_len<((double)input_len)*3.0/4.0+1.0)
		return false;
	memset(output, 0, output_len);
	//decode
	BIO * b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	BIO * bmem = BIO_new_mem_buf(const_cast<void*>(input), input_len);
	bmem = BIO_push(b64, bmem);
	output_len = BIO_read(bmem, output, output_len);
	//free resource
	BIO_free_all(bmem);
	return output_len>0;
}

bool _des_ecb_encrypt(void* output, uint32_t& output_len, const void* input, uint32_t input_len, const void* key)
{
	uint8_t pad_len = pkcs7_pad(input_len,8);
	uint32_t olen = input_len+pad_len;
	if(output_len<olen)
		return false;
	DES_key_schedule ks;
	const DES_cblock* bkey = (const DES_cblock*)key;
	DES_set_key_unchecked(const_cast<DES_cblock*>(bkey), &ks);
	uint32_t pos=0;
	for(; pos+8<=input_len; pos+=8)
	{
		DES_ecb_encrypt( (DES_cblock*)( ((uint8_t*)input)+pos ),
			(DES_cblock*)( ((uint8_t*)output)+pos ),
			&ks, DES_ENCRYPT);
	}
	//padding and encrypt the padded line
	DES_cblock in;
	memcpy(in, ((uint8_t*)input)+pos, 8-pad_len);
	for(uint32_t i=0; i<pad_len; ++i)
		in[8-pad_len+i] = pad_len;
	DES_cblock* out = (DES_cblock*)( ((uint8_t*)output)+pos );
	DES_ecb_encrypt(&in, out, &ks, DES_ENCRYPT);
	output_len = olen;
	return true;
}
bool _des_ecb_decrypt(void* output, uint32_t& output_len, const void* input, uint32_t input_len, const void* key)
{
	if(0!=input_len%8 || output_len<input_len-8)
		return false;
	DES_key_schedule ks;
	const DES_cblock* bkey = (const DES_cblock*)key;
	DES_set_key_unchecked(const_cast<DES_cblock*>(bkey), &ks);
	uint32_t pos=0;
	for(; pos+8<input_len; pos+=8)
	{
		DES_ecb_encrypt( (DES_cblock*)( ((uint8_t*)input)+pos ),
			(DES_cblock*)( ((uint8_t*)output)+pos ),
			&ks, DES_DECRYPT);
	}
	DES_cblock out;
	DES_ecb_encrypt( (DES_cblock*)( ((uint8_t*)input)+pos ), &out, &ks, DES_DECRYPT);
	if(output_len<input_len-out[7])
		return false;
	memcpy((uint8_t*)output+input_len-8, out, 8-out[7]);
	output_len = input_len-out[7];
	return true;
}
