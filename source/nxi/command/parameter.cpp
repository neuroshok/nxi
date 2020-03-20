#include <nxi/command/parameter.hpp>

namespace nxi
{
    command_parameter::command_parameter(QString name, nxi::command_function_parameters_type suggestion_callback = nullptr)
        : name_{ std::move(name) }
        , suggestion_callback_{ std::move(suggestion_callback) }
    {}

    const QString& command_parameter::name() const
    {
        return name_;
    }

    void command_parameter::suggestion_callback(nxi::suggestion_vector& v) const
    {
        if (suggestion_callback_) suggestion_callback_(v);
    }
} // nxi