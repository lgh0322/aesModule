#include <string.h>
#include "aes.h"





const static u8 Sbox[16][16] =    // populate the Sbox matrix
{
    /*     0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
    /*0*/{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
    /*1*/{ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
    /*2*/{ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
    /*3*/{ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
    /*4*/{ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
    /*5*/{ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
    /*6*/{ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
    /*7*/{ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
    /*8*/{ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
    /*9*/{ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
    /*a*/{ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
    /*b*/{ 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
    /*c*/{ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
    /*d*/{ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
    /*e*/{ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
    /*f*/{ 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};

const static u8 iSbox[16][16] =    // populate the iSbox matrix
{
    /*     0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
    /*0*/{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb },
    /*1*/{ 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb },
    /*2*/{ 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e },
    /*3*/{ 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 },
    /*4*/{ 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 },
    /*5*/{ 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 },
    /*6*/{ 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 },
    /*7*/{ 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b },
    /*8*/{ 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 },
    /*9*/{ 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e },
    /*a*/{ 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b },
    /*b*/{ 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 },
    /*c*/{ 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f },
    /*d*/{ 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef },
    /*e*/{ 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 },
    /*f*/{ 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
};
const static u8 Rcon[11][4] =   //轮常数表
{
    //
    { 0x00, 0x00, 0x00, 0x00 },  //
    { 0x01, 0x00, 0x00, 0x00 }, //
    { 0x02, 0x00, 0x00, 0x00 }, //
    { 0x04, 0x00, 0x00, 0x00 }, //
    { 0x08, 0x00, 0x00, 0x00 }, //
    { 0x10, 0x00, 0x00, 0x00 }, //
    { 0x20, 0x00, 0x00, 0x00 }, //
    { 0x40, 0x00, 0x00, 0x00 }, //
    { 0x80, 0x00, 0x00, 0x00 }, //
    { 0x1b, 0x00, 0x00, 0x00 }, //
    { 0x36, 0x00, 0x00, 0x00 }, //
};

static inline u8 gfmultby01(u8 b)    //乘1
{
    return b;
}

static inline u8 gfmultby02(u8 b)   //乘2
{
    if (b < 0x80)
    {
        return (b << 1);
    }
    else
    {
        return ((b << 1) ^ (0x1b));
    }
}

static inline u8 gfmultby03(u8 b)
{
    return (gfmultby02(b) ^ b); //GF域的加法运算就是异或
}

static inline u8 gfmultby09(u8 b)
{
    return (gfmultby02(gfmultby02(gfmultby02(b))) ^ b);
}

static inline u8 gfmultby0b(u8 b)
{
    return (gfmultby02(gfmultby02(gfmultby02(b))) ^ gfmultby02(b) ^ b);
}

static inline u8 gfmultby0d(u8 b)
{
    return (gfmultby02(gfmultby02(gfmultby02(b))) ^ gfmultby02(gfmultby02(b)) ^ (b));
}

static inline u8 gfmultby0e(u8 b)
{
    return (gfmultby02(gfmultby02(gfmultby02(b))) ^ gfmultby02(gfmultby02(b)) ^ gfmultby02(b));
}

static inline void RotWord4(u8 *ws)    //左旋一位
{
    u8 t = ws[0];
    ws[0] = ws[1];
    ws[1] = ws[2];
    ws[2] = ws[3];
    ws[3] = t;
}

static void SubWord4(u8 *ws)    //用替换表 Sbox 对一给定的一行密钥调度表 w[] 进行逐字节替换。
{
    ws[0] = Sbox[ws[0] >> 4][ws[0] & 0x0f];
    ws[1] = Sbox[ws[1] >> 4][ws[1] & 0x0f];
    ws[2] = Sbox[ws[2] >> 4][ws[2] & 0x0f];
    ws[3] = Sbox[ws[3] >> 4][ws[3] & 0x0f];
}

static void ShiftRows(u8 State[][4])
{
    u8 temp[4][4];
    memcpy(temp, State, sizeof(temp));

    for (int r = 1; r < 4; ++r)    // shift temp into State
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] = temp[r][(c + r) & AES_NB_MASK];
        }
    }
}

static void MixColumns(u8 State[][4])
{
    u8 temp[4][4];
    memcpy(temp, State, sizeof(temp));

    for (int c = 0; c < 4; ++c)
    {
        State[0][c] = (gfmultby02(temp[0][c]) ^ gfmultby03(temp[1][c]) ^ gfmultby01(temp[2][c]) ^ gfmultby01(temp[3][c]));
        State[1][c] = (gfmultby01(temp[0][c]) ^ gfmultby02(temp[1][c]) ^ gfmultby03(temp[2][c]) ^ gfmultby01(temp[3][c]));
        State[2][c] = (gfmultby01(temp[0][c]) ^ gfmultby01(temp[1][c]) ^ gfmultby02(temp[2][c]) ^ gfmultby03(temp[3][c]));
        State[3][c] = (gfmultby03(temp[0][c]) ^ gfmultby01(temp[1][c]) ^ gfmultby01(temp[2][c]) ^ gfmultby02(temp[3][c]));
    }
}

static void SubBytes(u8 State[][4])
{
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] = Sbox[(State[r][c] >> 4)][(State[r][c] & 0x0f)];
        }
    }
}

static void InvSubBytes(u8 State[][4])
{
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] = iSbox[(State[r][c] >> 4)][(State[r][c] & 0x0f)];
        }
    }
}

static void InvShiftRows(u8 State[][4])
{
    u8 temp[4][4];
    memcpy(temp, State, sizeof(temp));

    for (int r = 1; r < 4; ++r)    // shift temp into State
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][(c + r) & AES_NB_MASK] = temp[r][c];
        }
    }
}

static void InvMixColumns(u8 State[][4])
{
    u8 temp[4][4];
    memcpy(temp, State, sizeof(temp));

    for (int c = 0; c < 4; ++c)
    {
        State[0][c] = (gfmultby0e(temp[0][c]) ^ gfmultby0b(temp[1][c]) ^ gfmultby0d(temp[2][c]) ^ gfmultby09(temp[3][c]));
        State[1][c] = (gfmultby09(temp[0][c]) ^ gfmultby0e(temp[1][c]) ^ gfmultby0b(temp[2][c]) ^ gfmultby0d(temp[3][c]));
        State[2][c] = (gfmultby0d(temp[0][c]) ^ gfmultby09(temp[1][c]) ^ gfmultby0e(temp[2][c]) ^ gfmultby0b(temp[3][c]));
        State[3][c] = (gfmultby0b(temp[0][c]) ^ gfmultby0d(temp[1][c]) ^ gfmultby09(temp[2][c]) ^ gfmultby0e(temp[3][c]));
    }
}

static void AddRoundKey(int round, u8 State[][4], const u8 keyw[][4])
{
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            State[r][c] ^= keyw[(round * 4) + c][r];
        }
    }
}

/*********************************************************************************************
 * 名称: aes_initKey
 * 功能: 初始化AES密钥
 * 参数: AesKey* pkey: 打初始化的密钥
 * 参数: u8 *key: 密钥原始字节
 * 参数: int keysize: 密钥长度
 * 返回: boolean: 是否成功
 ********************************************************************************************/
int aes_initKey(AesKey* pkey, u8 *key, int keysize)
{
    int Nk;    //代表以字为单位的块长

    switch (keysize)
    {
        case 16:
        case 128:
            Nk = 4;
            pkey->nc = 10;
            pkey->len = 16;
            break;

        case 24:
        case 192:
            Nk = 6;
            pkey->nc = 12;
            pkey->len = 24;
            break;

        case 32:
        case 256:
            Nk = 8;
            pkey->nc = 14;
            pkey->len = 32;
            break;

        default:
            pkey->nc = 0;
            pkey->len = 0;
            return 0;
    }

//w[] 最初的 Nk (6) 行被作为种子，用原始密钥值
    for (int row = 0; row < Nk; ++row)
    {
        pkey->keyw[row][0] = key[4 * row];
        pkey->keyw[row][1] = key[4 * row + 1];
        pkey->keyw[row][2] = key[4 * row + 2];
        pkey->keyw[row][3] = key[4 * row + 3];
    }

    u8 temp[4];

    for (int row = Nk; row < AES_NB * (pkey->nc + 1); ++row)
    {
        temp[0] = pkey->keyw[row - 1][0];
        temp[1] = pkey->keyw[row - 1][1];
        temp[2] = pkey->keyw[row - 1][2];
        temp[3] = pkey->keyw[row - 1][3];

        if (row % Nk == 0)
        {
            RotWord4(temp);
            SubWord4(temp);
            temp[0] ^= Rcon[row - Nk][0];
            temp[1] ^= Rcon[row - Nk][1];
            temp[2] ^= Rcon[row - Nk][2];
            temp[3] ^= Rcon[row - Nk][3];
        }
        else if (Nk > 6 && (row % Nk == 4))
        {
            SubWord4(temp);
        }

        pkey->keyw[row][0] = (pkey->keyw[row - Nk][0] ^ temp[0]);
        pkey->keyw[row][1] = (pkey->keyw[row - Nk][1] ^ temp[1]);
        pkey->keyw[row][2] = (pkey->keyw[row - Nk][2] ^ temp[2]);
        pkey->keyw[row][3] = (pkey->keyw[row - Nk][3] ^ temp[3]);
    }

    return 1;
}

/*********************************************************************************************
 * 名称: aes_encrypt
 * 功能: AES加密
 * 参数: const AesKey* pkey: 初始化好的密钥
 * 参数: u8 *input: 输入数据
 * 参数: u8 *output: 输出数据
 * 返回: int: 数据长度
 ********************************************************************************************/
int aes_encrypt(const AesKey* pkey, u8 *input, u8 *output)
{
    if (pkey->nc <= 0)
    {
        return 0;
    }

    u8 State[4][AES_NB];

    for (int i = 0; i < (4 * AES_NB); ++i)
    {
        State[i & 3][i >> 2] = input[i];        //this->State[i % 4][i / 4] = input[i];
    }

    AddRoundKey(0, State, pkey->keyw);

    for (int round = 1; round <= (pkey->nc - 1); ++round)
    {
        SubBytes(State);
        ShiftRows(State);
        MixColumns(State);
        AddRoundKey(round, State, pkey->keyw);
    }

    SubBytes(State);
    ShiftRows(State);
    AddRoundKey(pkey->nc, State, pkey->keyw);

    for (int i = 0; i < (4 * AES_NB); ++i)
    {
        output[i] = State[i & 3][i >> 2];  //this->State[i % 4][i / 4];
    }

    return pkey->len;
}

/*********************************************************************************************
 * 名称: aes_decrypt
 * 功能: AES解密
 * 参数: const AesKey* pkey: 初始化好的密钥
 * 参数: u8 *input: 输入数据
 * 参数: u8 *output: 输出数据
 * 返回: int: 数据长度
 ********************************************************************************************/
int aes_decrypt(const AesKey* pkey, u8 *input, u8 *output)
{
    if (pkey->nc <= 0)
    {
        return 0;
    }

    u8 State[4][AES_NB];

    for (int i = 0; i < (4 * AES_NB); ++i)
    {
        State[i & 3][i >> 2] = input[i];
    }

    AddRoundKey(pkey->nc, State, pkey->keyw);

    for (int round = pkey->nc - 1; round >= 1; --round)
    {
        InvShiftRows(State);
        InvSubBytes(State);
        AddRoundKey(round, State, pkey->keyw);
        InvMixColumns(State);
    }

    InvShiftRows(State);
    InvSubBytes(State);
    AddRoundKey(0, State, pkey->keyw);

    for (int i = 0; i < (4 * AES_NB); ++i)
    {
        output[i] = State[i & 3][i >> 2];
    }

    return pkey->len;
}


