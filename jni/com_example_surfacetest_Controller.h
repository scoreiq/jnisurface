#include <jni.h>


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_test(
                  JNIEnv* env,
                  jobject thiz);

JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_drawNoise(
                  JNIEnv* env,
                  jobject thiz);

JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_startRender(
                  JNIEnv* env,
                  jobject thiz);

#ifdef __cplusplus
}
#endif
