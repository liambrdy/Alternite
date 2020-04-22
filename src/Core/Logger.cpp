#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::Init()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>("Alternite.log", true));

    sinks[0]->set_pattern("%^ [%R] %n: %v%$");
    sinks[1]->set_pattern("[%r] [%l] %n: %v");

    s_logger = std::make_shared<spdlog::logger>("Log", std::begin(sinks), std::end(sinks));
    spdlog::register_logger(s_logger);
    s_logger->set_level(spdlog::level::trace);
    s_logger->flush_on(spdlog::level::trace);
}