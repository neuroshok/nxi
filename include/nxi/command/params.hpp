#ifndef INCLUDE_NXI_COMMAND_PARAMS_HPP_NXI
#define INCLUDE_NXI_COMMAND_PARAMS_HPP_NXI

#include <vector>

#include <QString>
#include <nxi/log.hpp>

namespace nxi
{
	class command_params
    {
    public:
        command_params() = default;

        void add(const QString& value) { values_.push_back(value); }
        auto get(int index) const { nxi_assert(index < values_.size()); return values_[index]; }
        void clear() { values_.clear(); }

    public:
        std::vector<QString> values_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_PARAMS_HPP_NXI
