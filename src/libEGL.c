#include <dlfcn.h>
#include <EGL/egl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

struct ANativeWindow {
   GLFWwindow *glfw;
};

EGLDisplay
eglGetDisplay(EGLNativeDisplayType native_display)
{
   static union { EGLDisplay (*fun)(EGLNativeDisplayType); void *ptr; } orig;
   if (!orig.ptr) orig.ptr = dlsym(RTLD_NEXT, "eglGetDisplay");
   return orig.fun(glfwGetX11Display());
}

EGLSurface
eglCreateWindowSurface(EGLDisplay display, EGLConfig config, NativeWindowType native_window, EGLint const *attrib_list)
{
   static union { EGLSurface (*fun)(EGLDisplay, EGLConfig, NativeWindowType, EGLint const*); void *ptr; } orig;
   if (!orig.ptr) orig.ptr = dlsym(RTLD_NEXT, "eglCreateWindowSurface");
   struct ANativeWindow *win = (struct ANativeWindow*)native_window;
   return orig.fun(display, config, glfwGetX11Window(win->glfw), attrib_list);
}
