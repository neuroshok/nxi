#include <nxi/log.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#define nxi_(Message, ...) SPDLOG_LOGGER_TRACE(spdlog::get("execution"), Message, __VA_ARGS__)

void init_logger()
{
    bool log_source = true;
    auto log_level = spdlog::level::debug;

    std::string str_log_source = "";
    if (log_source) str_log_source = " (%@)";
    spdlog::set_level(log_level);

    auto debug_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    debug_sink->set_color(spdlog::level::debug, debug_sink->RED);
    auto debug_logger = std::make_shared<spdlog::logger>("debug", debug_sink);
    debug_logger->set_level(spdlog::level::debug);
    debug_logger->set_pattern("%^[%n]%$ [%!] %v" + str_log_source);
    spdlog::register_logger(debug_logger);

    auto execution_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    execution_sink->set_color(spdlog::level::trace, execution_sink->CYAN);
    auto execution_logger = std::make_shared<spdlog::logger>("execution", execution_sink);
    execution_logger->set_level(spdlog::level::trace);
    execution_logger->set_pattern("%^[%n]%$ [%!] %v" + str_log_source);
    spdlog::register_logger(execution_logger);

    auto event_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    event_sink->set_color(spdlog::level::trace, execution_sink->GREEN);
    auto event_logger = std::make_shared<spdlog::logger>("event", event_sink);
    event_logger->set_level(spdlog::level::trace);
    event_logger->set_pattern("%^[%n]%$ %v" + str_log_source);
    spdlog::register_logger(event_logger);
}