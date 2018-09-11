#include <jni.h>
#include <string>
#include "Rtmp.h"


#ifdef __cplusplus
extern "C" {
#endif

static jlong JNICALL
Publish_native_init(JNIEnv *env, jclass type, jstring url_, jint w, jint h,
                                         jint timeOut) {

    const char *url = env->GetStringUTFChars(url_, 0);
    Rtmp *rtmp = new Rtmp();
    rtmp->init(url, w, h, timeOut);
    env->ReleaseStringUTFChars(url_, url);
    return reinterpret_cast<long> (rtmp);
}


static jint JNICALL
Publish_native_sendSpsAndPps(JNIEnv *env, jclass type, jlong cptr,
                                                  jbyteArray sps_, jint spsLen, jbyteArray pps_,
                                                  jint ppsLen, jlong timestamp) {
    jbyte *sps = env->GetByteArrayElements(sps_, NULL);
    jbyte *pps = env->GetByteArrayElements(pps_, NULL);
    Rtmp *rtmp = reinterpret_cast<Rtmp *>(cptr);
    int ret = rtmp->sendSpsAndPps((BYTE *) sps, spsLen, (BYTE *) pps, ppsLen, timestamp);

    env->ReleaseByteArrayElements(sps_, sps, 0);
    env->ReleaseByteArrayElements(pps_, pps, 0);
    return ret;
}

static jint JNICALL
Publish_native_sendVideoData(JNIEnv *env, jclass type, jlong cptr,
                                                  jbyteArray data_, jint len, jlong timestamp) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);
    Rtmp *rtmp = reinterpret_cast<Rtmp *> (cptr);
    int ret = rtmp->sendVideoData((BYTE *) data, len, timestamp);

    env->ReleaseByteArrayElements(data_, data, 0);

    return ret;
}


static jint JNICALL
Publish_native_sendAacSpec(JNIEnv *env, jclass type, jlong cptr,
                                                jbyteArray data_, jint len) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);

    Rtmp *rtmp = reinterpret_cast<Rtmp *> (cptr);
    int ret = rtmp->sendAacSpec((BYTE *) data, len);

    env->ReleaseByteArrayElements(data_, data, 0);
    return ret;
}

static jint JNICALL
Publish_native_sendAacData(JNIEnv *env, jclass type, jlong cptr,
                                                jbyteArray data_, jint len, jlong timestamp) {
    jbyte *data = env->GetByteArrayElements(data_, NULL);

    Rtmp *rtmp = reinterpret_cast<Rtmp *> (cptr);
    int ret = rtmp->sendAacData((BYTE *) data, len, timestamp);

    env->ReleaseByteArrayElements(data_, data, 0);
    return ret;
}

static jint JNICALL
Publish_native_stop(JNIEnv *env, jclass type, jlong cptr) {
    Rtmp *rtmp = reinterpret_cast<Rtmp *> (cptr);
    delete rtmp;
    return 0;
}

#ifdef __cplusplus
}
#endif
static const JNINativeMethod gMethods[] = {
        {"Publish_init", "(Ljava/lang/String;III)J", (void*)Publish_native_init},
        {"Publish_sendSpsAndPps","(J[BI[BIJ)I",(void*)Publish_native_sendSpsAndPps},
        {"Publish_sendVideoData","(J[BIJ)I",(void*)Publish_native_sendVideoData},
        {"Publish_sendAacSpec","(J[BI)I",(void*)Publish_native_sendAacSpec},
        {"Publish_sendAacData","(J[BIJ)I",(void*)Publish_native_sendAacData},
        {"Publish_stop","(J)I",(void*)Publish_native_stop}
};
static jclass myClass;
//这里是java调用C的存在Native方法的类路径
static const char* const kClassName="com/exmple/cj/videoeditor/rtmp/PublishNative";
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv* env = NULL; //注册时在JNIEnv中实现的，所以必须首先获取它
    jint result = -1;
    if((vm)->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) //从JavaVM获取JNIEnv，一般使用1.4的版本
        return -1;
    myClass = (env)->FindClass( kClassName);
    if(myClass == NULL)
    {
        printf("cannot get class:%s\n", kClassName);
        return -1;
    }
    if((env)->RegisterNatives(myClass,gMethods,sizeof(gMethods)/sizeof(gMethods[0]))<0)
    {
        printf("register native method failed!\n");
        return -1;
    }
    printf("--------JNI_OnLoad-----");
    return JNI_VERSION_1_4; //这里很重要，必须返回版本，否则加载会失败。
}