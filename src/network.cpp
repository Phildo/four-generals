#include "network.h"

#include "logger.h"
#include <cstring>

#ifdef FG_ANDROID

#include <cstddef>
#include <jni.h>
// ../android-project/src/com/phildogames/fourgenerals/FourGeneralsActivity.java //here for vim gf

namespace AndroidNetwork
{
  static JavaVM *jvm;
  static jclass jc;

  String getIP()
  {
    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);

    jmethodID jm = env->GetStaticMethodID(jc, "getIP", "()Ljava/lang/String;");
    jstring js = (jstring)env->CallStaticObjectMethod(jc, jm);
    const char *s = env->GetStringUTFChars(js, NULL);
    String ret;
    if(s)
    {
      fg_log("In C, the received string is: %s", s);
      ret = String(s);
      env->ReleaseStringUTFChars(js, s);
    }

    return ret;
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
  JNIEXPORT void JNICALL Java_com_phildogames_fourgenerals_FourGeneralsActivity_setupEnv(JNIEnv* env, jclass jc)
  {
    env->GetJavaVM(&AndroidNetwork::jvm);
    AndroidNetwork::jc = (jclass)env->NewGlobalRef(jc);
  }
}

#endif


using namespace Network;

String Network::getIP()
{
#ifdef FG_ANDROID
  return AndroidNetwork::getIP();
#endif
#ifdef FG_PC
  struct ifaddrs *ap;
  struct ifaddrs *cur;
  int r;

  char host[NI_MAXHOST];
  String ip;

  r = getifaddrs(&ap);
  for(cur = ap; cur != NULL && cur->ifa_addr != NULL; cur = cur->ifa_next)
  {
    r = cur->ifa_addr->sa_family;

    if(r == AF_INET)
    {
      r = getnameinfo(cur->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if(strcmp(cur->ifa_name, "en0") == 0 || strcmp(cur->ifa_name, "eth0") == 0) ip = String(host, strlen(host));
    }
  }

 freeifaddrs(ap);
 return ip;
#endif
}


