#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "jvm/jni.h"

// System

#define PROP_NAME_MAX   32
#define PROP_VALUE_MAX  92

int
__system_property_get(const char *name, char *value)
{
   return 0;
}

// AConfiguration

struct AConfiguration {
   char nop;
};

struct AAssetManager {
   char nop;
};

struct AConfiguration*
AConfiguration_new(void)
{
   return calloc(1, sizeof(struct AConfiguration));
}

void
AConfiguration_delete(struct AConfiguration *config)
{
   free(config);
}

void
AConfiguration_fromAssetManager(struct AConfiguration *out, struct AAssetManager *am)
{
}

// ALooper

static struct ALooper {
   char nop;
} thread_looper;

typedef void* ALooper_callbackFunc;

struct ALooper*
ALooper_forThread(void)
{
   return &thread_looper;
}

struct ALooper*
ALooper_prepare(int opts)
{
   return &thread_looper;
}

void
ALooper_wake(struct ALooper *looper)
{
}

int
ALooper_pollAll(int timeoutMillis, int *outFd, int *outEvents, void **outData)
{
   return 0;
}

int
ALooper_pollOnce(int timeoutMillis, int *outFd, int *outEvents, void **outData)
{
   return 0;
}

// ASensor

struct ASensor {
   char nop;
};

const char*
ASensor_getName(struct ASensor const *sensor)
{
   return NULL;
}

const char*
ASensor_getVendor(struct ASensor const *sensor)
{
   return NULL;
}

int
ASensor_getType(struct ASensor const *sensor)
{
   return 0;
}

float
ASensor_getResolution(struct ASensor const *sensor)
{
   return 0;
}

int
ASensor_getMinDelay(struct ASensor const *sensor)
{
   return 0;
}

struct ASensorEvent {
   char nop;
};

struct ASensorEventQueue {
   char nop;
};

int
ASensorEventQueue_enableSensor(struct ASensorEventQueue *queue, struct ASensor const *sensor)
{
   return 0;
}

int
ASensorEventQueue_disableSensor(struct ASensorEventQueue *queue, struct ASensor const *sensor)
{
   return 0;
}

int
ASensorEventQueue_setEventRate(struct ASensorEventQueue *queue, struct ASensor const *sensor, int32_t usec)
{
   return 0;
}

int
ASensorEventQueue_hasEvents(struct ASensorEventQueue *queue)
{
   return 0;
}

size_t
ASensorEventQueue_getEvents(struct ASensorEventQueue *queue, struct ASensorEvent *events, size_t count)
{
   return 0;
}

struct ASensorManager {
   char nop;
};

struct ASensorList {
   char nop;
};

struct ASensorManager*
ASensorManager_getInstance()
{
   return NULL;
}

struct ASensor const*
ASensorManager_getDefaultSensor(struct ASensorManager *manager, int type)
{
   return NULL;
}

int
ASensorManager_getSensorList(struct ASensorManager *manager, struct ASensorList *list)
{
   return 0;
}

struct ASensorEventQueue*
ASensorManager_createEventQueue(struct ASensorManager *manager, struct ALooper *looper, int ident, ALooper_callbackFunc callback, void *data)
{
   return NULL;
}

int
ASensorManager_destroyEventQueue(struct ASensorManager *manager, struct ASensorEventQueue *queue)
{
   return 0;
}

// AInput

struct AInputEvent {
   char nop;
};

int32_t
AInputEvent_getType(const struct AInputEvent *event)
{
   return 0;
}

int32_t
AInputEvent_getDeviceId(const struct AInputEvent *event)
{
   return 0;
}

int32_t
AInputEvent_getSource(const struct AInputEvent *event)
{
   return 0;
}

int32_t
AMotionEvent_getEdgeFlags(const struct AInputEvent *motion_event)
{
   return 0;
}

float
AMotionEvent_getTouchMajor(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

float
AMotionEvent_getTouchMinor(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

int32_t
AMotionEvent_getPointerId(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

int64_t
AMotionEvent_getDownTime(const struct AInputEvent *motion_event)
{
   return 0;
}

size_t
AMotionEvent_getPointerCount(const struct AInputEvent *motion_event)
{
   return 0;
}

float
AMotionEvent_getXPrecision(const struct AInputEvent *motion_event)
{
   return 0;
}

float
AMotionEvent_getYPrecision(const struct AInputEvent *motion_event)
{
   return 0;
}

float
AMotionEvent_getToolMajor(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

float
AMotionEvent_getToolMinor(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

float
AMotionEvent_getX(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

float
AMotionEvent_getY(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

float
AMotionEvent_getSize(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

float
AMotionEvent_getPressure(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

int64_t
AMotionEvent_getEventTime(const struct AInputEvent *motion_event)
{
   return 0;
}

int32_t
AMotionEvent_getFlags(const struct AInputEvent *motion_event)
{
   return 0;
}

int32_t
AMotionEvent_getMetaState(const struct AInputEvent *motion_event)
{
   return 0;
}

int32_t
AMotionEvent_getAction(const struct AInputEvent *motion_event)
{
   return 0;
}

size_t
AMotionEvent_getHistorySize(const struct AInputEvent *motion_event)
{
   return 0;
}

int64_t
AMotionEvent_getHistoricalEventTime(const struct AInputEvent *motion_event, size_t history_index)
{
   return 0;
}

size_t
AMotionEvent_getHistoricalSize(const struct AInputEvent *motion_event, size_t pointer_index, size_t history_index)
{
   return 0;
}

float
AMotionEvent_getHistoricalPressure(const struct AInputEvent *motion_event, size_t pointer_index, size_t history_index)
{
   return 0;
}

float
AMotionEvent_getHistoricalX(const struct AInputEvent *motion_event, size_t pointer_index, size_t history_index)
{
   return 0;
}

float
AMotionEvent_getHistoricalY(const struct AInputEvent *motion_event, size_t pointer_index, size_t history_index)
{
   return 0;
}

float
AMotionEvent_getOrientation(const struct AInputEvent *motion_event, size_t pointer_index)
{
   return 0;
}

int32_t
AKeyEvent_getKeyCode(const struct AInputEvent *key_event)
{
   return 0;
}

int32_t
AKeyEvent_getMetaState(const struct AInputEvent *key_event)
{
   return 0;
}

int32_t
AKeyEvent_getAction(const struct AInputEvent *key_event)
{
   return 0;
}

struct AInputQueue {
   char nop;
};

void
AInputQueue_attachLooper(struct AInputQueue *queue, struct ALooper *looper, int ident, ALooper_callbackFunc callback, void *data)
{
}

void
AInputQueue_detachLooper(struct AInputQueue *queue)
{
}

int32_t
AInputQueue_hasEvents(struct AInputQueue *queue)
{
   return 0;
}

int32_t
AInputQueue_getEvent(struct AInputQueue *queue, struct AInputEvent **outEvent)
{
   return 0;
}

int32_t
AInputQueue_preDispatchEvent(struct AInputQueue *queue, struct AInputEvent *event)
{
   return 0;
}

void
AInputQueue_finishEvent(struct AInputQueue *queue, struct AInputEvent *event, int handled)
{
}

// ANative

struct ANativeWindow {
   GLFWwindow *glfw;
};

enum {
   WINDOW_FORMAT_RGBA_8888          = 1,
   WINDOW_FORMAT_RGBX_8888          = 2,
   WINDOW_FORMAT_RGB_565            = 4,
};

struct ANativeWindow_Buffer {
   // The number of pixels that are show horizontally.
   int32_t width;

   // The number of pixels that are shown vertically.
   int32_t height;

   // The number of *pixels* that a line in the buffer takes in
   // memory.  This may be >= width.
   int32_t stride;

   // The format of the buffer.  One of WINDOW_FORMAT_*
   int32_t format;

   // The actual bits.
   void* bits;

   // Do not touch.
   uint32_t reserved[6];
};

struct ARect {
   int32_t left, top, right, bottom;
};

static void
glfw_error_cb(int code, const char *error)
{
   fprintf(stderr, "glfw: (%d) %s\n", code, error);
}

struct ANativeWindow*
ANativeWindow_fromSurface(JNIEnv* env, jobject surface)
{
   assert(env && surface);

   struct ANativeWindow *window;
   if (!(window = calloc(1, sizeof(*window))))
      return NULL;

   glfwInit();
   fprintf(stderr, "glfw: %s\n", glfwGetVersionString());
   glfwSetErrorCallback(glfw_error_cb);
   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
   window->glfw = glfwCreateWindow(1024, 768, "ANativeWindow", NULL, NULL);
   assert(window->glfw);
   return window;
}

void
ANativeWindow_acquire(struct ANativeWindow *window)
{
   assert(window);
   // FIXME: refcount
}

void
ANativeWindow_release(struct ANativeWindow *window)
{
   assert(window);
   // FIXME: refcount
}

int32_t
ANativeWindow_getWidth(struct ANativeWindow *window)
{
   assert(window);
   int v = 0;
   glfwGetWindowSize(window->glfw, &v, NULL);
   return v;
}

int32_t
ANativeWindow_getHeight(struct ANativeWindow *window)
{
   assert(window);
   int v = 0;
   glfwGetWindowSize(window->glfw, NULL, &v);
   return v;
}

int32_t
ANativeWindow_setBuffersGeometry(struct ANativeWindow *window, int32_t width, int32_t height, int32_t format)
{
   assert(window);
   return 0;
}

int32_t
ANativeWindow_lock(struct ANativeWindow *window, struct ANativeWindow_Buffer *outBuffer, struct ARect *inOutDirtyBounds)
{
   assert(window);
   *outBuffer = (struct ANativeWindow_Buffer){0};
   return 0;
}

int32_t
ANativeWindow_unlockAndPost(struct ANativeWindow *window)
{
   assert(window);
   return 0;
}
