/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_gxengine_core_GAndroidApp */

#ifndef _Included_com_gxengine_core_GAndroidApp
#define _Included_com_gxengine_core_GAndroidApp
#ifdef __cplusplus
extern "C" {
#endif
#undef com_gxengine_core_GAndroidApp_BIND_ABOVE_CLIENT
#define com_gxengine_core_GAndroidApp_BIND_ABOVE_CLIENT 8L
#undef com_gxengine_core_GAndroidApp_BIND_ADJUST_WITH_ACTIVITY
#define com_gxengine_core_GAndroidApp_BIND_ADJUST_WITH_ACTIVITY 128L
#undef com_gxengine_core_GAndroidApp_BIND_ALLOW_OOM_MANAGEMENT
#define com_gxengine_core_GAndroidApp_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef com_gxengine_core_GAndroidApp_BIND_AUTO_CREATE
#define com_gxengine_core_GAndroidApp_BIND_AUTO_CREATE 1L
#undef com_gxengine_core_GAndroidApp_BIND_DEBUG_UNBIND
#define com_gxengine_core_GAndroidApp_BIND_DEBUG_UNBIND 2L
#undef com_gxengine_core_GAndroidApp_BIND_EXTERNAL_SERVICE
#define com_gxengine_core_GAndroidApp_BIND_EXTERNAL_SERVICE -2147483648L
#undef com_gxengine_core_GAndroidApp_BIND_IMPORTANT
#define com_gxengine_core_GAndroidApp_BIND_IMPORTANT 64L
#undef com_gxengine_core_GAndroidApp_BIND_NOT_FOREGROUND
#define com_gxengine_core_GAndroidApp_BIND_NOT_FOREGROUND 4L
#undef com_gxengine_core_GAndroidApp_BIND_WAIVE_PRIORITY
#define com_gxengine_core_GAndroidApp_BIND_WAIVE_PRIORITY 32L
#undef com_gxengine_core_GAndroidApp_CONTEXT_IGNORE_SECURITY
#define com_gxengine_core_GAndroidApp_CONTEXT_IGNORE_SECURITY 2L
#undef com_gxengine_core_GAndroidApp_CONTEXT_INCLUDE_CODE
#define com_gxengine_core_GAndroidApp_CONTEXT_INCLUDE_CODE 1L
#undef com_gxengine_core_GAndroidApp_CONTEXT_RESTRICTED
#define com_gxengine_core_GAndroidApp_CONTEXT_RESTRICTED 4L
#undef com_gxengine_core_GAndroidApp_MODE_APPEND
#define com_gxengine_core_GAndroidApp_MODE_APPEND 32768L
#undef com_gxengine_core_GAndroidApp_MODE_ENABLE_WRITE_AHEAD_LOGGING
#define com_gxengine_core_GAndroidApp_MODE_ENABLE_WRITE_AHEAD_LOGGING 8L
#undef com_gxengine_core_GAndroidApp_MODE_MULTI_PROCESS
#define com_gxengine_core_GAndroidApp_MODE_MULTI_PROCESS 4L
#undef com_gxengine_core_GAndroidApp_MODE_NO_LOCALIZED_COLLATORS
#define com_gxengine_core_GAndroidApp_MODE_NO_LOCALIZED_COLLATORS 16L
#undef com_gxengine_core_GAndroidApp_MODE_PRIVATE
#define com_gxengine_core_GAndroidApp_MODE_PRIVATE 0L
#undef com_gxengine_core_GAndroidApp_MODE_WORLD_READABLE
#define com_gxengine_core_GAndroidApp_MODE_WORLD_READABLE 1L
#undef com_gxengine_core_GAndroidApp_MODE_WORLD_WRITEABLE
#define com_gxengine_core_GAndroidApp_MODE_WORLD_WRITEABLE 2L
#undef com_gxengine_core_GAndroidApp_IDLE_MS_PE_RFRAME
#define com_gxengine_core_GAndroidApp_IDLE_MS_PE_RFRAME 16LL
/*
 * Class:     com_gxengine_core_GAndroidApp
 * Method:    jniOnCreate
 * Signature: (Ljava/lang/ClassLoader;)V
 */
JNIEXPORT void JNICALL Java_com_gxengine_core_GAndroidApp_jniOnCreate
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_gxengine_core_GAndroidApp
 * Method:    jniMain
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_gxengine_core_GAndroidApp_jniMain
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_gxengine_core_GAndroidApp
 * Method:    jniOnTerminate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_gxengine_core_GAndroidApp_jniOnTerminate
  (JNIEnv *, jobject);

/*
 * Class:     com_gxengine_core_GAndroidApp
 * Method:    jniOnLowMemory
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_gxengine_core_GAndroidApp_jniOnLowMemory
  (JNIEnv *, jobject);

/*
 * Class:     com_gxengine_core_GAndroidApp
 * Method:    jniIdle
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_gxengine_core_GAndroidApp_jniIdle
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
