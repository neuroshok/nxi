#ifndef NXI_LOG_H_NXI
#define NXI_LOG_H_NXI

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <QString>

void init_logger();

template<typename OStream>
OStream& operator<<(OStream &os, const QString& c)
{
    return os << c.toStdString();
}

#if defined(__GNUC__) || defined(__clang__)
    #define nxi_unreachable() void((__builtin_unreachable())
#elif defined(_MSC_VER) && _MSC_VER >= 1900
    #define nxi_unreachable() void(__assume(false))
#else
    #define nxi_unreachable() void()
#endif

#define nxi_assert(C) Q_ASSERT(C)
#define nxi_debug(Message, ...) SPDLOG_LOGGER_DEBUG(spdlog::get("debug"), Message, __VA_ARGS__)
#define nxi_error(Message, ...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), Message, __VA_ARGS__)
#define nxi_warning(Message, ...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), Message, __VA_ARGS__)

#define nxi_trace(Message, ...) SPDLOG_LOGGER_TRACE(spdlog::get("execution"), Message, __VA_ARGS__)
#define nxi_trace_event(Message, ...) SPDLOG_LOGGER_TRACE(spdlog::get("event"), Message, __VA_ARGS__)

#endif // NXI_LOG_H_NXI