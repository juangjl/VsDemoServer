/**
 * @file Aes.h
 * @brief  Software AES256 related function
 * @version $revision$
 * @author Ryan Yeh<ryan_yeh@vsigntek.com>
 * @note Copyright 2024, VitalSigns Technology Co., Ltd., all rights reserved.
 * @note
*/ 
#ifndef _AES_H_
#define _AES_H_

#include "Global.h"

///--------------------------------------------------------------------------------------------------------------------------------///
///
/// Macros definitions
///
///--------------------------------------------------------------------------------------------------------------------------------///

#define AES_CBC       (1)
#define AES_ECB       (1)
#define AES_CTR       (1)

//#define AES128            (1)
//#define AES192            (1)
#define AES256              (1)

#define AES_BLOCK_LEN                     (16) ///< Block length in bytes - AES is 128b block only
#define AES_DATA_SIZE                     (2048)

#if defined(AES256) && (AES256 == 1) ///< for AES 256
    #define AES_KEYLEN                    (32)
    #define AES_keyExpSize                (240)
#elif defined(AES192) && (AES192 == 1) ///< for AES 192
    #define AES_KEYLEN                    (24)
    #define AES_keyExpSize                (208)
#else ///< for AES 128
    #define AES_KEYLEN                    (16)   ///< Key length in bytes
    #define AES_keyExpSize                (176)
#endif ///< AES type selection

///--------------------------------------------------------------------------------------------------------------------------------///
///
/// Context for AES operations
///
///--------------------------------------------------------------------------------------------------------------------------------///
typedef struct AesCtxST
{
  JBYTE RoundKey[AES_keyExpSize];  
  JBYTE Iv[AES_BLOCK_LEN];  
} AesCtx; 

///--------------------------------------------------------------------------------------------------------------------------------///
///
/// Funcitons
///
///--------------------------------------------------------------------------------------------------------------------------------///
extern void  AesCtxInit(AesCtx * ctx, const JBYTE * key);
extern void  AecCtxIVInit(AesCtx * ctx, const JBYTE * key, const JBYTE * iv);
extern void  AesCtxIVSet(AesCtx * ctx, const JBYTE * iv);

extern void  AesEcbEncrypt(AesCtx * ctx, JBYTE * buf);
extern void  AesEcbDecrypt(AesCtx * ctx, JBYTE * buf);

extern void  AesEcbBufferEncrypt(AesCtx * ctx, JBYTE * buf, JINT iLenTotal);
extern JBOOL AesEcbBufferDecrypt(AesCtx * ctx, JBYTE * buf, JINT iLenTotal);


extern void  AesCbcBufferEncrypt(AesCtx * ctx, JBYTE * buf, size_t length);
extern void  AesCbcBufferDecrypt(AesCtx * ctx, JBYTE * buf, size_t length);

extern void  AesCtrBufferXcrypt(AesCtx * ctx, JBYTE * buf, size_t length);

#endif ///< for _AES_H_

