#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>

#include "com_example_surfacetest_Controller.h"

#define WIDTH 400
#define HEIGHT	400

static jbyte* g_buffer = NULL;
static uint8_t* pixelBuffer;

static JavaVM*	gJVM;
static JNIEnv* gEnv;
static jobject gJavaObject = NULL;
static jmethodID gRenderCallback = NULL;

static pthread_t	renderThread;
static int			renderThreadId;
static bool 		isAttached = false;
static JNIEnv*		threadsJNIEnv;

bool jni_surface_init(JNIEnv* env, jobject obj) {
	return JNI_VERSION_1_6;
}

void generateNoise()
{
	for(int i = 0 ; i < WIDTH * HEIGHT * 2 ; i++)
	{
		pixelBuffer[i] = (uint8_t)rand();
	}
}

void jni_surface_release(JNIEnv* env) {

	jclass javaClass = env->GetObjectClass(gJavaObject);
	jmethodID releaseMethod = env->GetMethodID(javaClass,"surfaceRelease", "()V");

	env->CallVoidMethod(javaClass, releaseMethod);
	g_buffer = NULL;

	if(pixelBuffer) delete[] pixelBuffer;
}


void jni_surface_render(JNIEnv* env, uint8_t* pixels, int w, int h) {
	if (g_buffer == NULL) return;
	memcpy(g_buffer, pixels, w * h * 2); // RGB565 pixels
	env->CallVoidMethod(gJavaObject, gRenderCallback);
}

void* renderFrame(void* attributes)
{
	__android_log_print(ANDROID_LOG_DEBUG, "native", "renderFrame");
	if(!isAttached)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "native", "renderFrame: attaching thread.");
		gJVM->AttachCurrentThread(&threadsJNIEnv, NULL);
		__android_log_print(ANDROID_LOG_DEBUG, "native", "renderFrame: attaching thread - Attached.");
		isAttached = true;
	}

	while(true)
	{
		generateNoise();
		__android_log_print(ANDROID_LOG_DEBUG, "native", "renderFrame: noise generated.");
		jni_surface_render(threadsJNIEnv, pixelBuffer, WIDTH, HEIGHT);
	}


	gJVM->DetachCurrentThread();
}

JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_test(
                  JNIEnv* env,
                  jobject thiz)
{
	__android_log_print(ANDROID_LOG_DEBUG, "native", "Hello JNI. jobj is %d", thiz);
	env->GetJavaVM(&gJVM);

	gEnv = env;
	gJavaObject = env->NewGlobalRef(thiz);

	jclass objectsClass = env->GetObjectClass(thiz);
	__android_log_print(ANDROID_LOG_DEBUG, "native", "test: objectsClass = %d", objectsClass);

	jmethodID callback = env->GetMethodID(objectsClass,"initSurface", "()Ljava/nio/ByteBuffer;");
	__android_log_print(ANDROID_LOG_DEBUG, "native", "test: callback = %d", callback);

	gRenderCallback = env->GetMethodID(objectsClass, "surfaceRender", "()V");
	__android_log_print(ANDROID_LOG_DEBUG, "native", "test: gRenderCallback = %d", gRenderCallback);

	jobject buff = env->CallObjectMethod(thiz,callback);
	__android_log_print(ANDROID_LOG_DEBUG, "native", "test: buff = %d", buff);

	g_buffer = (jbyte*)env->GetDirectBufferAddress(buff);
	//if (g_buffer == NULL) return false;
	__android_log_print(ANDROID_LOG_DEBUG, "native", "g_buff = %d", g_buffer);

	pixelBuffer = new uint8_t[WIDTH * HEIGHT * 2];


}


JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_startRender(
                  JNIEnv* env,
                  jobject thiz)
{
	renderThreadId = pthread_create(&renderThread, NULL, renderFrame, NULL);
	__android_log_print(ANDROID_LOG_DEBUG, "native", "renderThread = %d", renderThread);
	pthread_join(renderThread, NULL);
}


JNIEXPORT void JNICALL Java_com_example_surfacetest_NativeRenderer_drawNoise(JNIEnv* env, jobject thiz)
{
	generateNoise();
	jni_surface_render(env, pixelBuffer, WIDTH, HEIGHT);
}

