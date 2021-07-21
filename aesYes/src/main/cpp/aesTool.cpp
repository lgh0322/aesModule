#include <jni.h>
#include <string>
#include "aes.h"





//int main(int argc, char **argv) {

//   printDatas("AES:", os, sizeof(os));  //AES:E0D541314E00102D6DFCA8BC007B6C8A
//
//   aes_decrypt(&pkey, is, os);
//   printDatas("AES:", os, sizeof(os));  //AES:5E749C2D64E5EF78279337670223FC08
//}




extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_vaca_aes128native_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    jbyteArray ret = env->NewByteArray(16);
    u8 ks[16];
   memset(ks, 0x32, sizeof(ks));

   AesKey pkey;
   aes_initKey(&pkey, ks, 16);

   u8 is[16]={78,53,10,115,131,126,178,232,226,142,222,13,86,201,48,69};
   u8 os[16];
    aes_decrypt(&pkey, is, os);
//   memset(is, 0, sizeof(is));
//   memset(os, 0, sizeof(os));
//
//   aes_encrypt(&pkey, is, os);



    env->SetByteArrayRegion (ret, 0, 16, reinterpret_cast<const jbyte *>(os));
    return ret;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_vaca_aesyes_AES_encry(JNIEnv *env, jobject thiz, jbyteArray key, jbyteArray content) {
    // TODO: implement encry()
}