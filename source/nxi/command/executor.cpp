#include <nxi/command/executor.hpp>

#include <nxi/command.hpp>

namespace nxi
{
    command_executor::command_executor(nds::node_ptr<const nxi::command> command)
        : complete_{ false }
        , command_{ command }
        , parameter_index_{ 0 }
    {}

    void command_executor::add_value(const QString& value)
    {
        values_.add(value);
    }

    void command_executor::exec()
    {
        if (complete_)
        {
            nxi_warning("command already executed");
            return;
        }

        ++parameter_index_;
        if (is_ready())
        {
            command_->exec(values_);
            complete_ = true;
        }
    }

    void command_executor::reset()
    {
        complete_ = false;
        parameter_index_ = 0;
    }


    const nxi::command& command_executor::command() const { return *command_; }

    const nxi::command_parameter& command_executor::active_parameter() const
    {
        return command_->parameter(parameter_index_);
    }

    bool command_executor::is_ready() const
    {
        return parameter_index_ >= command_->parameters_count();
    }

    bool command_executor::is_complete() const { return complete_; }
} // nxi