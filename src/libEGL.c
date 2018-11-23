#include <dlfcn.h>
#include <string.h>
#include <EGL/egl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

struct ANativeWindow {
   char header[4];
   GLFWwindow *glfw;
};

EGLSurface
eglCreateWindowSurface(EGLDisplay display, EGLConfig config, NativeWindowType native_window, EGLint const *attrib_list)
{
   static union { EGLSurface (*fun)(EGLDisplay, EGLConfig, NativeWindowType, EGLint const*); void *ptr; } orig;
   if (!orig.ptr) orig.ptr = dlsym(RTLD_NEXT, "eglCreateWindowSurface");
   struct ANativeWindow *window = (struct ANativeWindow*)native_window;
   return orig.fun(display, config, (!memcmp(window->header, "andr", sizeof(window->header)) ? glfwGetX11Window(window->glfw) : native_window), attrib_list);
}
