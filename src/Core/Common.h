#pragma once

#include "Logger.h"

#include <signal.h>

#ifndef NDEBUG
    #define ASSERTS_ENABLED
#endif

#ifdef ASSERTS_ENABLED
    #define ASSERT(x, ...) { if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); }}
#else
    #define ASSERT(x, ...)
#endif