#pragma once

#include "Logger.h"

#include <signal.h>

#include <memory>

#ifndef NDEBUG
    #define ASSERTS_ENABLED
#endif

#ifdef ASSERTS_ENABLED
    #define ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); }}
#else
    #define ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Scope = std::unique_ptr<T>;