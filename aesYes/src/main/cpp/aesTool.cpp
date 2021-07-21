#include <jni.h>
#include <string>
#include "aes.h"





//int main(int argc, char **argv) {

//   printDatas("AES:", os, sizeof(os));  //AES:E0D541314E00102D6DFCA8BC007B6C8A
//
//   aes_decrypt(&pkey, is, os);
//   printDatas("AES:", os, sizeof(os));  //AES:5E749C2D64E5EF78279337670223FC08
//}





extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_vaca_aesyes_AES_encry(JNIEnv *env, jobject thiz, jbyteArray key, jbyteArray content,jint num) {
    signed char *aeskey = env->GetByteArrayElements(key, NULL);
    signed char *inputContent = env->GetByteArrayElements(content, NULL);
    AesKey pkey;
    aes_initKey(&pkey, reinterpret_cast<u8 *>(aeskey), 16);
    char * res= static_cast<char *>(malloc(num));
    jbyteArray ret = env->NewByteArray(num);
    u8 is[16];
    u8 os[16];
    for(int k=0;k<num;k+=16){
        memcpy(is,inputContent+k,16);
        aes_encrypt(&pkey,is,os);
        for(int j=0;j<16;j++){
            res[k+j]=os[j];
        }
    }

    env->ReleaseByteArrayElements(key, aeskey, 0);
    env->ReleaseByteArrayElements(content, inputContent, 0);
    env->SetByteArrayRegion (ret, 0, num, reinterpret_cast<const jbyte *>(res));
    free(res);
    return ret;
}


extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_vaca_aesyes_AES_decry(JNIEnv *env, jobject thiz, jbyteArray key, jbyteArray content,
                               jint num) {
    signed char *aeskey = env->GetByteArrayElements(key, NULL);
    signed char *inputContent = env->GetByteArrayElements(content, NULL);
    AesKey pkey;
    aes_initKey(&pkey, reinterpret_cast<u8 *>(aeskey), 16);
    char * res= static_cast<char *>(malloc(num));
    jbyteArray ret = env->NewByteArray(num);
    u8 is[16];
    u8 os[16];
    for(int k=0;k<num;k+=16){
        memcpy(is,inputContent+k,16);
        aes_decrypt(&pkey,is,os);
        for(int j=0;j<16;j++){
            res[k+j]=os[j];
        }
    }

    env->ReleaseByteArrayElements(key, aeskey, 0);
    env->ReleaseByteArrayElements(content, inputContent, 0);
    env->SetByteArrayRegion (ret, 0, num, reinterpret_cast<const jbyte *>(res));
    free(res);
    return ret;
}