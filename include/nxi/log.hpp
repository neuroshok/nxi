#ifndef NXI_LOG_H_NXI
#define NXI_LOG_H_NXI

#include <nxi/error.hpp>
#include <iostream>

#include <spdlog/spdlog.h>

#define nxi_assert(C) Q_ASSERT(C)

#define nxi_log std::cout << std::endl << "[nxi] "

class logger
{
public:
    logger() {}

private:

};


#endif // NXI_LOG_H_NXI