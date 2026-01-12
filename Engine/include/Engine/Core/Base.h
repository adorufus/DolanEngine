#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <iostream>
#include <sstream>
#include <cstdint>

// Platform detection
#ifdef _WIN32
    #ifdef _WIN64
        #define GE_PLATFORM_WINDOWS
    #else
        #error "x86 builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC == 1
        #define GE_PLATFORM_MACOS
    #else
        #error "Unknown Apple platform!"
    #endif
#elif defined(__linux__)
    #define GE_PLATFORM_LINUX
#else
    #error "Unknown platform!"
#endif

// Debug macros
#ifdef GE_DEBUG
    #if defined(GE_PLATFORM_WINDOWS)
        #define GE_DEBUGBREAK() __debugbreak()
    #elif defined(GE_PLATFORM_MACOS) || defined(GE_PLATFORM_LINUX)
        #include <csignal>
        #define GE_DEBUGBREAK() std::raise(SIGTRAP)
    #endif
    #define GE_ENABLE_ASSERTS
#else
    #define GE_DEBUGBREAK()
#endif

// Assert macros
#ifdef GE_ENABLE_ASSERTS
    #define GE_CORE_ASSERT(x, msg) \
        { \
            if(!(x)) { \
                GE_CORE_ERROR("Assertion Failed: {0}", msg); \
                GE_DEBUGBREAK(); \
            } \
        }
    #define GE_ASSERT(x, msg) \
        { \
            if(!(x)) { \
                GE_ERROR("Assertion Failed: {0}", msg); \
                GE_DEBUGBREAK(); \
            } \
        }
#else
    #define GE_CORE_ASSERT(x, msg)
    #define GE_ASSERT(x, msg)
#endif

// Bit manipulation
#define BIT(x) (1 << x)

// Smart pointers
namespace Engine {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

