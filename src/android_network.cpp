#include <cstddef>
#include <jni.h>
// ../android-project/src/com/phildogames/fourgenerals/FourGeneralsActivity.java //here for vim gf

namespace AndroidNetwork
{
  static JavaVM *jvm;
  static jclass jc;

  void construct()
  {

  }

  void broadcast(char *c, int l)
  {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);

    char cs[l+1];
    for(int i = 0; i < l; i++)
      cs[i] = c[i];
    cs[l] = 0;

    jstring js = env->NewStringUTF(cs);
    jmethodID jm = env->GetStaticMethodID(jc, "broadcast", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(jc, jm, js);
  }

  void destruct()
  {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    env->DeleteGlobalRef(jc);
  }
}

extern "C"
{
  JNIEXPORT void JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_initNativeEnv(JNIEnv* env, jclass jc)
  {
    env->GetJavaVM(&AndroidNetwork::jvm);
    AndroidNetwork::jc = (jclass)env->NewGlobalRef(jc);
  }

  //THIS MAINLY SERVES AS AN EXAMPLE FOR PHIL'S OWN REFERENCE. DON'T ACTUALLY CALL.
  JNIEXPORT jstring JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_cFunc(JNIEnv* env, jclass jc, jstring js)
  {
    fg_log("C Func!");

    //save jvm
    env->GetJavaVM(&AndroidNetwork::jvm);

    /*
    Assuming we didn't have env...

    //reattach thread
    JNIEnv *env;
    AndroidNetwork::jvm->AttachCurrentThread(&env, NULL);

    */

    //echo passed in string
    const char *s = env->GetStringUTFChars(js, NULL);
    fg_log("In C, the received string is: %s\n", s);
    env->ReleaseStringUTFChars(js, s);

    //call java func with string
    char cs[] = "String from C to Java (local)";
    jstring js2 = env->NewStringUTF(cs);
    jmethodID jm = env->GetStaticMethodID(jc, "jFunc", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(jc, jm, js2);

    //return string
    char cs2[] = "String from C to Java (returned)";
    return env->NewStringUTF(cs2);
  }
}

