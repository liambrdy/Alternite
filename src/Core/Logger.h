#pragma once

#include <spdlog/spdlog.h>

#include <memory>

class Logger
{
public:
    static void Init();

    static std::shared_ptr<spdlog::logger> GetLogger() { return s_logger; }
private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

#define LOG_TRACE(...) Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::GetLogger()->critical(__VA_ARGS__)