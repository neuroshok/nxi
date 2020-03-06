#include <nxi/command.hpp>
#include <nxi/command/params.hpp>

#include <nxi/suggestion_vector.hpp>

#include <nxi/log.hpp>

namespace nxi
{
    command::command(nxi::command_data data)
        : module_name_{ std::move(data.module) }
        , node_{ nullptr }
        , action_name_ { std::move(data.action) }
        , name_ { module_name_ + ":" + action_name_ }
        , function_{ std::move(data.function) }
        , icon_ { std::move(data.icon) }
        , description_ { std::move(data.description) }
        , shortcut_ { std::move(data.shortcut) }
        , preview_ { data.preview }
    {}

    command::command(nds::node<nxi::command> *, nxi::command_data)
    {

    }

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
        if (function_) function_(nxi::command_params{});
    }

    void command::exec(const nxi::command_params& params) const
    {
        if (function_) function_(params);
    }

    void command::add_param(const QString& name, std::function<void(nxi::suggestion_vector&)> fn)
    {
        params_.push_back(name);
        param_suggestions_ = std::move(fn);
    }

    void command::add_suggestion(nxi::suggestion_vector& p) const
    {
        param_suggestions_(p);
    }


    void command::set_function(command::function_type fn)
    {
        function_ = std::move(fn);
    }

    void command::set_node(nds::node<nxi::command>* node)
    {
        node_ = node;
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