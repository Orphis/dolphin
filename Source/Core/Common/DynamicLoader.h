#pragma once

#if __linux__ || __APPLE__ || __FreeBSD__

#include <functional>
#include <iostream>
#include <string>

#include <dlfcn.h>

#define DL_IMPORT_FUNC(x) std::function<decltype(::x)> x = cppdlsym<decltype(::x)>(#x);

class DynamicLoaderBase {
 public:
  explicit DynamicLoaderBase(std::string name, int flag = RTLD_NOW) {
#if defined(__linux__)
    name = "lib" + name + ".so";
#elif defined(__APPLE__)
    name = "lib" + name + ".dylib";
#endif

    handle = dlopen(name.c_str(), flag);
    if(!handle) {
      std::cerr << "Failed to dlopen \"" << name << "\"" << std::endl;
    }
  }

  virtual ~DynamicLoaderBase() {
  }

  bool IsLoaded() const {
    return handle != nullptr;
  }

  explicit operator bool() const
  {
    return IsLoaded();
  }
 protected:
  void *handle;

  template <typename Func>
  std::function<Func> cppdlsym(std::string name) {
    auto f = reinterpret_cast<Func*>(dlsym(handle, name.c_str()));
    if(!f) {
      std::cerr << "Failed to dlsym \"" << name << "\" in handle " << handle << ": " << dlerror() << std::endl;
    }
    return f;
  }
};

#endif
