#pragma once

#include "jni.h"
#include <stdbool.h>

struct jvm_string {
   const char *data;
   size_t size;
   bool heap;
};

struct jvm_array {
   void *data;
   size_t element_sz, size; // size == in elements, size * element_sz for bytes
};

struct jvm_class {
   struct jvm_string name;
};

struct jvm_method {
   jclass klass;
   struct jvm_string name, signature;
};

struct jvm_object {
   union {
      struct jvm_array array;
      struct jvm_method method;
      struct jvm_class klass;
      struct jvm_string string;
   };
   enum jvm_object_type {
      JVM_OBJECT_NONE,
      JVM_OBJECT_ARRAY,
      JVM_OBJECT_METHOD,
      JVM_OBJECT_CLASS,
      JVM_OBJECT_STRING,
   } type;
};

struct jvm_native_method {
   struct jvm_method method;
   void *function;
};

struct jvm {
   struct jvm_object objects[256];
   struct jvm_native_method methods[255];

   struct JNINativeInterface native;
   struct JNIInvokeInterface invoke;

   // JNI's api is weird.. pointer to a reference of a struct, OK!
   // Developers have to dereference these pointers to call methods from an ... reference.
   // NOTE: These are pointers, and JNI interface passes pointers to these pointers!
   JNIEnv env; // points to native
   JavaVM vm; // points to invoke
};

void
jvm_init(struct jvm *jvm);
