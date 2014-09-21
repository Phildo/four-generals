#include <cstddef>
#include <jni.h>
// ../android-project/src/com/phildogames/fourgenerals/FourGeneralsActivity.java //here for vim gf

extern "C"
{
  static JavaVM *jvm;

  JNIEXPORT jstring JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_cFunc(JNIEnv* env, jclass jc, jstring js)
  {
    fg_log("C Func!");

    //save jvm
    env->GetJavaVM(&jvm);

    /*
    Assuming we didn't have env...

    //reattach thread
    JNIEnv *env;
    (*jvm)->AttachCurrentThread(&env, NULL);

    */

    //echo passed in string
    const char *s = env->GetStringUTFChars(js, NULL);
    fg_log("In C, the received string is: %s\n", s);
    env->ReleaseStringUTFChars(js, s);

    //call java fuc with string
    char cs[] = "String from C to Java (local)";
    jstring js2 = env->NewStringUTF(cs);
    jmethodID jm = env->GetStaticMethodID(jc, "jFunc", "(Ljava/lang/String;)V");
    env->CallStaticVoidMethod(jc, jm, js2);

    //return string
    char cs2[] = "String from C to Java (returned)";
    return env->NewStringUTF(cs2);
  }
}

