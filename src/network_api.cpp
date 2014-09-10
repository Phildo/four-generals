#include <string.h>
#include <stdio.h>
#include <jni.h>
#include "logger.h"

extern "C" {
JNIEXPORT void JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_cFunc(JNIEnv *, jclass);
JNIEXPORT void JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_cFunc(JNIEnv* env, jclass jc)
{
  fg_log("C Func!");
  /*
  //create jstring
  jstring jstr = (*env)->NewStringUTF(env, "This comes from jni.");

  //find class
  jclass clazz = (*env)->FindClass(env, "com/inceptix/android/t3d/MainActivity");
  //find method
  jmethodID messageMe = (*env)->GetMethodID(env, clazz, "messageMe", "(Ljava/lang/String;)Ljava/lang/String;");

  //call method, get result
  jobject result = (*env)->CallObjectMethod(env, obj, messageMe, jstr);

  //convert result to string
  jstring 
  */

  /*
  const char* str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
  (*env)->ReleaseStringUTFChars((jstring) result, str);

  return (*env)->NewStringUTF(env, str);
  */
}
}

