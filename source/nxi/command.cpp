#include <nxi/command.hpp>

namespace nxi
{
    command::command(const QString& module_name, const QString& action_name, function_type fn, const QString& icon)
        : module_name_{ module_name }
        , action_name_{ action_name }
        , name_{ module_name_ + ":" + action_name_ }
        , icon_{ icon }
        , function_{ std::move(fn) }
    {

    }

    void command::exec() const
    {
        if (function_) function_(nxi::command_params{});
    }

    void command::exec(const nxi::command_params& params) const
    {
        if (function_) function_(params);
    }

    void command::add_param(const QString& name, std::vector<QString> suggestions)
    {
        params_.push_back(name);
        param_suggestions_.push_back(std::move(suggestions));
    }

    const std::vector<QString>& command::param_suggestions(int index)
    {
        return param_suggestions_[index];
    }


    command::params_type command::params() const
    {
        return params_;
    }

    const QString& command::name() const
    {
        return name_;
    }

    const QString& command::module_name() const
    {
        return module_name_;
    }

    const QString& command::action_name() const
    {
        return action_name_;
    }

    const QString& command::icon() const
    {
        return icon_;
    }

    const command::function_type& command::function() const
    {
        return function_;
    }
} // nxi