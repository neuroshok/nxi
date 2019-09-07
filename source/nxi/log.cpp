#include <nxi/log.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef _WIN32
    #define NXI_CONSOLE_COLOR_RED FOREGROUND_RED
    #define NXI_CONSOLE_COLOR_CYAN FOREGROUND_GREEN | FOREGROUND_BLUE
    #define NXI_CONSOLE_COLOR_GREEN FOREGROUND_GREEN
#else
    #define NXI_CONSOLE_COLOR_RED "\e[0;31m"
    #define NXI_CONSOLE_COLOR_CYAN "\e[0;36m"
    #define NXI_CONSOLE_COLOR_GREEN "\e[0;32m"
#endif

void init_logger()
{
    bool log_source = true;
    auto log_level = spdlog::level::debug;

    std::string str_log_source = "";
    if (log_source) str_log_source = " (%@)";
    spdlog::set_level(log_level);

    auto debug_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    debug_sink->set_color(spdlog::level::debug, NXI_CONSOLE_COLOR_RED);
    auto debug_logger = std::make_shared<spdlog::logger>("debug", debug_sink);
    debug_logger->set_level(spdlog::level::debug);
    debug_logger->set_pattern("%^[%n]%$ [%!] %v" + str_log_source);
    spdlog::register_logger(debug_logger);

    auto execution_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    execution_sink->set_color(spdlog::level::trace, NXI_CONSOLE_COLOR_CYAN);
    auto execution_logger = std::make_shared<spdlog::logger>("execution", execution_sink);
    execution_logger->set_level(spdlog::level::trace);
    execution_logger->set_pattern("%^[%n]%$ [%!] %v" + str_log_source);
    spdlog::register_logger(execution_logger);

    auto event_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    event_sink->set_color(spdlog::level::trace, NXI_CONSOLE_COLOR_GREEN);
    auto event_logger = std::make_shared<spdlog::logger>("event", event_sink);
    event_logger->set_level(spdlog::level::trace);
    event_logger->set_pattern("%^[%n]%$ %v" + str_log_source);
    spdlog::register_logger(event_logger);
}