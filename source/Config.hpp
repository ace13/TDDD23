#pragma once

#if defined(_WIN32)
#define WINDOWS
#elif defined(__LINUX__)
#define POSIX
#endif

#if !defined(NDEBUG) || defined(_DEBUG)
#define DEBUG
#endif

// Visual Studio 2010 does not have a working C++11 foreach, so let's fake it
#if defined(WINDOWS) && !(_MSC_VER > 1600)
#define FOR_EACH(a, b) for (auto __##b##__it = b.begin(); __##b##__it != b.end(); ++__##b##__it) if (bool __##b##__cont = true) for (a = *__##b##__it; __##b##__cont; __##b##__cont = false)
#else
#define FOR_EACH(a, b) for (a : b)
#endif
