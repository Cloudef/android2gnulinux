#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

struct ANativeWindow {
   GLFWwindow *glfw;
};

EGLDisplay
eglGetDisplay(EGLNativeDisplayType native_display)
{
   static EGLDisplay (*orig_eglGetDisplay)(EGLNativeDisplayType native_display);
   if (!orig_eglGetDisplay) orig_eglGetDisplay = dlsym(RTLD_NEXT, "eglGetDisplay");
   return orig_eglGetDisplay(glfwGetX11Display());
}

EGLSurface
eglCreateWindowSurface(EGLDisplay display, EGLConfig config, NativeWindowType native_window, EGLint const *attrib_list)
{
   static EGLSurface (*orig_eglCreateWindowSurface)(EGLDisplay display, EGLConfig config, NativeWindowType native_window, EGLint const *attrib_list);
   if (!orig_eglCreateWindowSurface) orig_eglCreateWindowSurface = dlsym(RTLD_NEXT, "eglCreateWindowSurface");
   struct ANativeWindow *win = (struct ANativeWindow*)native_window;
   return orig_eglCreateWindowSurface(display, config, glfwGetX11Window(win->glfw), attrib_list);
}
