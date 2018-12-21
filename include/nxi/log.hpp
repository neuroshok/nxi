#ifndef NXI_LOG_H_NXI
#define NXI_LOG_H_NXI

#include <nxi/error.hpp>
#include <iostream>

#define nxi_assert(C) Q_ASSERT(C)

#define nxi_log std::cout << std::endl << "[nxi] "



#endif // NXI_LOG_H_NXI