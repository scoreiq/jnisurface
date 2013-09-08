#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <jni.h>
#include <android/log.h>

#include "com_example_surfacetest_Controller.h"

#define WIDTH 200
#define HEIGHT	200

static JNIEnv*	jniEnv = NULL;
static jbyte* g_buffer = NULL;
static uint8_t* pixelBuffer;
static jobject javaObject = NULL;
static jmethodID renderMethod = NULL;

bool jni_surface_init(JNIEnv* env, jobject obj) {
	__android_log_print(ANDROID_LOG_DEBUG, "native", "jni_surface_init, jobj is %d", obj);
	jclass javaClass = env->GetObjectClass(obj);
	//jclass javaClass = env->FindClass("com/example/surfacetest/NativeRenderer");
	__android_log_print(ANDROID_LOG_DEBUG, "native", "jClass is %d", javaClass);


	if(env->IsSameObject(javaClass, javaObject)) __android_log_print(ANDROID_LOG_DEBUG, "native", "javaClass == javaObject");
	else __android_log_print(ANDROID_LOG_DEBUG, "native", "javaClass != javaObject");

	jmethodID initMethod = env->GetMethodID(env->GetObjectClass(javaObject),"initSurface", "()Ljava/nio/ByteBuffer;");
	__android_log_print(ANDROID_LOG_DEBUG, "native", "jmethod(init) is %d", initMethod);


	jobject buf = env->CallObjectMethod(javaObject, initMethod);
	if (buf == NULL) return false;
	__android_log_print(ANDROID_LOG_DEBUG, "native", "buf is ok.");

	g_buffer = (jbyte*)env->GetDirectBufferAddress(buf);
	if (g_buffer == NULL) return false;
	__android_log_print(ANDROID_LOG_DEBUG, "native", "g_buff is ok");

	pixelBuffer = new uint8_t[WIDTH * HEIGHT * 2];

	return JNI_VERSION_1_6;
}

void jni_surface_release(JNIEnv* env) {

	jclass javaClass = env->GetObjectClass(javaObject);
	jmethodID releaseMethod = env->GetMethodID(javaClass,"surfaceRelease", "()V");

	env->CallVoidMethod(javaClass, releaseMethod);
	g_buffer = NULL;
}

void jni_surface_render(JNIEnv* env, uint8_t* pixels, int w, int h) {
	__android_log_print(ANDROID_LOG_DEBUG, "native", "jni_surface_render");
	if (g_buffer == NULL) return;

	jclass javaClass = env->GetObjectClass(javaObject);
	renderMethod = env->GetMethodID(javaClass,"surfaceRender", "()V");

	memcpy(g_buffer, pixels, w * h * 2); // RGB565 pixels
	env->CallVoidMethod(javaClass, renderMethod);
}


JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_test(
                  JNIEnv* env,
                  jobject thiz)
{
	__android_log_print(ANDROID_LOG_DEBUG, "native", "Hello JNI. jobj is %d", thiz);
	jniEnv = env;

	if(javaObject == NULL)
	{
		jclass	cls = env->GetObjectClass(thiz);
		if(cls == NULL) return;

		javaObject = env->NewGlobalRef(cls);
		if(javaObject == NULL) return;

		bool res = env->IsSameObject(cls, javaObject);

		if(res) __android_log_print(ANDROID_LOG_DEBUG, "native", "cls == javaObject");
		else __android_log_print(ANDROID_LOG_DEBUG, "native", "cls != javaObject");
	}

	__android_log_print(ANDROID_LOG_DEBUG, "native", "Env:%d, javaObject(gRef):%d", *jniEnv, javaObject);

	jni_surface_init(env, thiz);
}

JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_drawNoise(JNIEnv* env, jobject thiz)
{
	__android_log_print(ANDROID_LOG_DEBUG, "native", "Draw noise");
	for(int i = 0 ; i < WIDTH * HEIGHT * 2 ; i++)
	{
		pixelBuffer[i] = (uint8_t)rand();
		jni_surface_render(env, pixelBuffer, WIDTH, HEIGHT);
	}
}

