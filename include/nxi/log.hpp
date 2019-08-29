#ifndef NXI_LOG_H_NXI
#define NXI_LOG_H_NXI

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <QString>

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
#define nxi_error(Message, ...) spdlog::error(Message, __VA_ARGS__);
#define nxi_warning(Message, ...) spdlog::warn(Message, __VA_ARGS__)
#define nxi_trace(Message, ...) spdlog::trace(Message, __VA_ARGS__)

#endif // NXI_LOG_H_NXI