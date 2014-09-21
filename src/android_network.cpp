#include <cstddef>
#include <jni.h>
// ../android-project/src/com/phildogames/fourgenerals/FourGeneralsActivity.java //here for vim gf

extern "C"
{
  JNIEXPORT jstring JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_cFunc(JNIEnv* env, jclass jc, jstring js)
  {
    fg_log("C Func!");

    //echo passed in string
    const char *s = env->GetStringUTFChars(js, NULL);
    fg_log("In C, the received string is: %s\n", s);
    env->ReleaseStringUTFChars(js, s);

    //call java fuc with string
    jstring js2 = env->NewStringUTF(env, "String from C to Java (local)");
    jmethodID jm = env->GetStaticMethodID(env, jc, "jFunc", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(env, jc, jm, js2);

    //return string
    char cs[22] = "String from C to Java (returned)";
    return env->NewStringUTF(cs);
  }
}

