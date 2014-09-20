#include <cstddef>
#include <jni.h>
// ../android-project/src/com/phildogames/fourgenerals/FourGeneralsActivity.java //here for vim gf

extern "C"
{
  JNIEXPORT jstring JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_cFunc(JNIEnv* env, jclass jc, jstring js)
  {
    fg_log("C Func!");

    const char *s = env->GetStringUTFChars(js, NULL);
    fg_log("In C, the received string is: %s\n", s);
    env->ReleaseStringUTFChars(js, s);

    char cs[22] = "String from C to Java";
    return env->NewStringUTF(cs);
  }
}
