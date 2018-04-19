#pragma once

void*
wrapper_create(const char *const symbol, void *function);

void
wrapper_set_cpp_demangler(void *function);
