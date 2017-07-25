#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H


////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__)

// Windows
#    define SYSTEM_WINDOWS
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif
#    ifndef NOMINMAX
#        define NOMINMAX
#    endif
#    include <windows.h>
#    include <winsock2.h>
#    define strcasecmp _stricmp
#    define strncasecmp _strnicmp

#elif defined(linux) || defined(__linux)

// Linux
#    define SYSTEM_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

// MacOS
#    define SYSTEM_MACOS

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

// FreeBSD
#    define SYSTEM_FREEBSD

#else

// Unsupported system
#    error This operating system is not supported by this library

#endif


////////////////////////////////////////////////////////////
// Identify the endianess
////////////////////////////////////////////////////////////
#if defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
        defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || defined(__sparc__) || defined(__Ha__)

// Big endian
#    define SYSTEM_ENDIAN_BIG

#else

// Little endian
#    define SYSTEM_ENDIAN_LITTLE

#endif

#include <stdint.h>
#include <climits>
#include <cstdio>
#include <cstdlib>
#undef __STRICT_ANSI__
#include <cstring>
#define __STRICT_ANSI__
#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <queue>
#include <list>
#include <tuple>
#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>
#include <functional>
#include <algorithm>
#include <array>
#include <random>
#include <ctime>

#ifdef _MSC_VER

#    pragma warning(disable : 4251)

#    if _MSC_VER < 1600
        namespace std {
            using namespace std::tr1;
            template <typename T>
            inline T& move(T& t) { return t; }
        }

#        define unique_ptr auto_ptr
#    endif
#    define strtoull _strtoui64
#    define strtoll _strtoi64

#    define ATTR_PACKED()
#    define FINLINE
#    define THREAD_LOCAL __declspec(thread)

#else

#    define ATTR_PACKED() __attribute__((packed))

#    if defined(__MINGW32__)
#        define FINLINE inline
#    else
#        define FINLINE inline __attribute__((always_inline))
#    endif
#    define THREAD_LOCAL __thread

#endif

#if defined(_WIN32) || defined(__WIN32__)

#    if defined(_MSC_VER)
#        define I64_FMT "I64"
#        define localtime_r(t,m)   localtime_s(m,t)
#    elif defined(__LP64__)
#        define I64_FMT "l"
#    else
#        define I64_FMT "ll"
#    endif
#    define snprintf           _snprintf 
#    define vsnprintf          _vsnprintf

#    include <io.h>

#else

#    if defined(__APPLE__)
#        define I64_FMT "q"
#    elif defined(__LP64__)
#        define I64_FMT "l"
#    else
#        define I64_FMT "ll"
#    endif
#    include <unistd.h>

#endif

#define SAFE_DELETE(obj) do { delete(obj); obj = nullptr; } while(0)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) private: \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)


#endif // SYSTEM_CONFIG_H
