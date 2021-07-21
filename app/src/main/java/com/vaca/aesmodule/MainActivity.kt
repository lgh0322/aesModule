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
        val fuck2=ByteArray(16){
            it.toByte()
        }
        val gg=AES.encry(fuck,fuck2,16)
        for(k in gg){
            Log.e("fuck",k.toUByte().toInt().toString())
        }
        val xx=AES.decry(fuck,gg,16)
        for(k in xx){
            Log.e("fuck",k.toUByte().toInt().toString())
        }
    }
}