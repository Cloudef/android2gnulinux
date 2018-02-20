#pragma once

void dl_parse_library_path(const char *path, char *delim);
void *bionic_dlopen(const char *filename, int flag);
const char *bionic_dlerror(void);
void *bionic_dlsym(void *handle, const char *symbol);
int bionic_dlclose(void *handle);
