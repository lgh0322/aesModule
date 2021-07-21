package com.vaca.aesmodule

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import com.vaca.aesyes.AES

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val fuck=ByteArray(16){
            it.toByte()
        }
        val fuck2=ByteArray(160){
            it.toByte()
        }
        val gg=AES.encry(fuck,fuck2,160)
//        for(k in gg){
//            Log.e("fuck",k.toUByte().toInt().toString())
//        }
//        fuck[15]=0.toByte()
        val xx=AES.decry(fuck,gg,160)
        for(k in xx){
            Log.e("fuck",k.toUByte().toInt().toString())
        }
    }
}