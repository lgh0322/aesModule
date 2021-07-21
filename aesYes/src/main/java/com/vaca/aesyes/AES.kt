package com.vaca.aesyes

object AES {
    init {
        System.loadLibrary("aes-lib")
    }

    external fun encry(key:ByteArray,content:ByteArray):ByteArray
}