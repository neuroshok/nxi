#ifndef NXI_LOG_H_NXI
#define NXI_LOG_H_NXI

#include <nxi/error.hpp>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <QString>

template<typename OStream>
OStream& operator<<(OStream &os, const QString& c)
{
    return os << c.toStdString();
}

#define nxi_assert(C) Q_ASSERT(C)

#define nxi_warning(Message, ...) spdlog::warn(Message, __VA_ARGS__)

#define nxi_trace(Message, ...) spdlog::trace(Message, __VA_ARGS__)


#endif // NXI_LOG_H_NXI