#ifndef INCLUDE_NXI_DATABASE_FIELD_HPP_NXI
#define INCLUDE_NXI_DATABASE_FIELD_HPP_NXI

namespace nxi
{
    template<unsigned int Index, class T>
    struct field
    {
        using type = T;
        inline static constexpr unsigned int index = Index;
    };
} // nxi

#endif // INCLUDE_NXI_DATABASE_FIELD_HPP_NXI
