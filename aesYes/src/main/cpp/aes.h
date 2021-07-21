#ifdef  __cplusplus
extern "C" {
#endif


#ifndef AES_H
#define AES_H

#define AES_NB (4)
#define AES_NB_MASK (AES_NB-1)



typedef unsigned char u8;
typedef signed long u32;

typedef struct
{
    int nc; //轮数  10 12 14
    int len; //密钥长度 16 24 32
    u8 keyw[AES_NB * (14 + 1)][4];   //为密钥调度表
} AesKey;


/*********************************************************************************************
 * 名称: aes_initKey
 * 功能: 初始化AES密钥
 * 参数: AesKey* pkey: 打初始化的密钥
 * 参数: u8 *key: 密钥原始字节
 * 参数: int keysize: 密钥长度
 * 返回: boolean: 是否成功
 ********************************************************************************************/
int aes_initKey(AesKey* pkey, u8 *key, int keysize);


/*********************************************************************************************
 * 名称: aes_encrypt
 * 功能: AES加密
 * 参数: const AesKey* pkey: 初始化好的密钥
 * 参数: u8 *input: 输入数据
 * 参数: u8 *output: 输出数据
 * 返回: int: 数据长度
 ********************************************************************************************/
int aes_encrypt(const AesKey* pkey, u8 *input, u8 *output);

/*********************************************************************************************
 * 名称: aes_decrypt
 * 功能: AES解密
 * 参数: const AesKey* pkey: 初始化好的密钥
 * 参数: u8 *input: 输入数据
 * 参数: u8 *output: 输出数据
 * 返回: int: 数据长度
 ********************************************************************************************/
int aes_decrypt(const AesKey* pkey, u8 *input, u8 *output);
#ifdef  __cplusplus
}
#endif
#endif