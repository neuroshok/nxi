#ifndef NXI_LOG_H_NXI
#define NXI_LOG_H_NXI

#include <nxi/error.hpp>
#include <iostream>

#include <spdlog/spdlog.h>

#define nxi_assert(C) Q_ASSERT(C)

#define nxi_trace(Message, ...) spdlog::trace(Message, __VA_ARGS__)


#endif // NXI_LOG_H_NXI