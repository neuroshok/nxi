#ifndef INCLUDE_NXI_VALUES_HPP_NXI
#define INCLUDE_NXI_VALUES_HPP_NXI

#include <nxi/command/parameter.hpp>
#include <nxi/value.hpp>

#include <vector>

#include <QString>
#include <nxi/log.hpp>

namespace nxi
{
	class values
    {
    public:
        values() = default;

        void add(const nxi::value& value) { values_.emplace_back( value ); }
        auto get(size_t index) const { nxi_assert(index < values_.size()); return values_[index]; }
        void clear() { values_.clear(); }
        size_t size() const { return values_.size(); }

    public:
        std::vector<nxi::value> values_;
    };
} // nxi

#endif // INCLUDE_NXI_VALUES_HPP_NXI
