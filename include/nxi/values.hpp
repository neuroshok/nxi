#ifndef INCLUDE_NXI_VALUES_HPP_NXI
#define INCLUDE_NXI_VALUES_HPP_NXI

#include <nxi/value.hpp>

#include <vector>

namespace nxi
{
    class values
    {
    public:
        values() = default;

        void add(const nxi::value& value);
        const nxi::value& get(size_t index) const;
        void clear();
        size_t size() const;

    public:
        std::vector<nxi::value> values_;
    };
} // nxi

#endif // INCLUDE_NXI_VALUES_HPP_NXI