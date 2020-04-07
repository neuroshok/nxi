#include <nxi/command.hpp>

#include <nxi/log.hpp>
#include <nxi/suggestion/vector.hpp>
#include <nxi/values.hpp>

namespace nxi
{
    command::command(nxi::command_data data)
        : module_name_{ std::move(data.module) }
        , node_{ nullptr }
        , action_name_ { std::move(data.action) }
        , name_ { module_name_ + ":" + action_name_ }
        , params_ { std::move(data.parameters) }
        , function_{ std::move(data.function) }
        , icon_ { std::move(data.icon) }
        , description_ { std::move(data.description) }
        , shortcut_ { std::move(data.shortcut) }
        , preview_ { data.preview }
    {}

    command::command(const QString& module_name, const QString& action_name, function_type fn, const QString& icon)
        : module_name_{ module_name }
        , node_{ nullptr }
        , action_name_{ action_name }
        , name_{ module_name_ + ":" + action_name_ }
        , icon_{ icon }
        , function_{ std::move(fn) }
        , preview_ { false }
    {}

    void command::exec() const
    {
        if (params_.size() > 0) nxi_error("command require parameters");
        exec(nxi::values{});
    }

    void command::exec(const nxi::values& params) const
    {
        if (function_) function_(params);
    }

    void command::add_param(const QString& name, std::function<void(nxi::suggestion_vector&)> fn)
    {
        params_.emplace_back(nxi::command_parameter{ name, std::move(fn) });
    }

    const command_parameter& command::parameter(unsigned int index) const
    {
        nxi_assert(index < params_.size());
        return params_[index];
    }

    unsigned int command::parameters_count() const
    {
        return params_.size();
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

    const QString& command::description() const
    {
        return description_;
    }

    const nxi::shortcut& command::shortcut() const
    {
        return shortcut_;
    }

    bool command::preview() const
    {
        return preview_;
    }
} // nxi