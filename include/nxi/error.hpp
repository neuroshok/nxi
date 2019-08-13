#ifndef NXI_ERROR_H_NXI
#define NXI_ERROR_H_NXI

#if defined(__GNUC__) || defined(__clang__)
    #define nxi_unreachable() void((__builtin_unreachable())
#elif defined(_MSC_VER) && _MSC_VER >= 1900
    #define nxi_unreachable() void(__assume(false))
#else
    #define nxi_unreachable() void()
#endif

#define nxi_error(M) qFatal(M)

#define nxi_assert(C) Q_ASSERT(C)


#endif // ERROR_H_NXI