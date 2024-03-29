#ifndef INCLUDE_NXI_UTILITY_HPP_NXI
#define INCLUDE_NXI_UTILITY_HPP_NXI

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

#if defined(__GNUC__) || defined(__clang__)
    #define nxi_unreachable() void(__builtin_unreachable())
#elif defined(_MSC_VER) && _MSC_VER >= 1900
    #define nxi_unreachable() void(__assume(false))
#else
    #define nxi_unreachable() void()
#endif

#endif // INCLUDE_NXI_UTILITY_HPP_NXI
