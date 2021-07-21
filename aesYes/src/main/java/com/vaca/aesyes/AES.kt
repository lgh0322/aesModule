package com.vaca.aesyes

object AES {
    init {
        System.loadLibrary("aes-lib")
    }
    external fun encry(key:ByteArray,content:ByteArray,num:Int):ByteArray
    external fun decry(key:ByteArray,content:ByteArray,num:Int):ByteArray

}