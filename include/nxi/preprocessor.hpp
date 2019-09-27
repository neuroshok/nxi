#ifndef INCLUDE_NXI_PREPROCESSOR_HPP_NXI
#define INCLUDE_NXI_PREPROCESSOR_HPP_NXI

// msvc fix (va_args not expanded)
#define nxm_expand_call(MACRO, ARGS) MACRO ARGS

// macro overload
#define nxm_overloaded(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME, ...) NAME
#define nxm_overload(MACRO, ...) nxm_expand_call(nxm_overloaded, (__VA_ARGS__, \
    MACRO##9, MACRO##8, MACRO##7, MACRO##6, MACRO##5, MACRO##4, MACRO##3, MACRO##2, MACRO))(__VA_ARGS__)

#endif // INCLUDE_NXI_PREPROCESSOR_HPP_NXI
