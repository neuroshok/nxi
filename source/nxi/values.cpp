#include <nxi/values.hpp>

#include <nxi/log.hpp>

namespace nxi
{
    void values::add(const nxi::value& value) { values_.emplace_back( value ); }
    const nxi::value& values::get(size_t index) const { nxi_assert(index < values_.size()); return values_[index]; }
    void values::clear() { values_.clear(); }
    size_t values::size() const { return values_.size(); }
} // nxi