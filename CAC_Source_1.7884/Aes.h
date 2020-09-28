/* Aes.h -- AES encryption / decryption
2009-11-23 : Igor Pavlov : Public domain */

/* Cut-down version for little endian only */

#ifndef __AES_H
#define __AES_H

#ifdef __cplusplus
extern "C"
{
#endif

#define AES_BLOCK_SIZE 16
typedef unsigned UInt32;
typedef unsigned char Byte;
#define GetUi32(p) (*(const UInt32 *) (p))
#define SetUi32(p, d) (*(UInt32 *)(p) = (d))

/* Call AesGenTables one time before other AES functions */
void AesGenTables(void);

/* UInt32 pointers must be 16-byte aligned */

/* 16-byte (4 * 32-bit words) blocks: 1 (IV) + 1 (keyMode) + 15 (AES-256 roundKeys) */
#define AES_NUM_IVMRK_WORDS ((1 + 1 + 15) * 4)

/* aes - 16-byte aligned pointer to keyMode+roundKeys sequence */
/* keySize = 16 or 24 or 32 (bytes) */
void Aes_SetKey_Enc(UInt32 *aes, const Byte *key, unsigned keySize);
void Aes_SetKey_Dec(UInt32 *aes, const Byte *key, unsigned keySize);

void Aes_Encode(const UInt32 *w, UInt32 *dest, const UInt32 *src);
void Aes_Decode(const UInt32 *w, UInt32 *dest, const UInt32 *src);

/* ivAes - 16-byte aligned pointer to iv+keyMode+roundKeys sequence: UInt32[AES_NUM_IVMRK_WORDS] */

#ifdef __cplusplus
}
#endif

#endif
