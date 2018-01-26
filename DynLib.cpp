#include "DynLib.h"

#if defined(WIN32)
#include "Windows.h"
#elif defined(__unix__) || defined(__APPLE__)
#include "dlfcn.h"
#endif

sys::DynLib::DynLib() {
#if defined(WIN32) || defined(__unix__) || defined(__APPLE__)
    handle_ = NULL;
#endif
}

sys::DynLib::DynLib(const char *name) {
#if defined(WIN32)
    handle_ = LoadLibraryA(name);
#elif defined(__unix__) || defined(__APPLE__)
    handle_ = dlopen(name, RTLD_LAZY);
#endif
}

sys::DynLib::DynLib(DynLib &&rhs) {
#if defined(WIN32) || defined(__unix__) || defined(__APPLE__)
    handle_ = rhs.handle_;
    rhs.handle_ = NULL;
#endif
}

sys::DynLib &sys::DynLib::operator=(DynLib &&rhs) {
#if defined(WIN32)
    if (handle_) {
        FreeLibrary(handle_);
        handle_ = NULL;
    }
    handle_ = rhs.handle_;
    rhs.handle_ = NULL;
#elif defined(__unix__) || defined(__APPLE__)
    if (handle_) {
        dlclose(handle_);
        handle_ = NULL;
    }
    handle_ = rhs.handle_;
    rhs.handle_ = NULL;
#endif
    return *this;
}

sys::DynLib::~DynLib() {
#if defined(WIN32)
    FreeLibrary(handle_);
    handle_ = NULL;
#elif defined(__unix__) || defined(__APPLE__)
    if (handle_) {
        dlclose(handle_);
        handle_ = NULL;
    }
#endif
}

sys::DynLib::operator bool() const {
#if defined(WIN32) || defined(__unix__) || defined(__APPLE__)
    return handle_ != NULL;
#endif
}

void *sys::DynLib::GetProcAddress(const char *name) {
#if defined(WIN32)
    return (void *)::GetProcAddress(handle_, name);
#elif defined(__unix__) || defined(__APPLE__)
    return dlsym(handle_, name);
#endif
}
