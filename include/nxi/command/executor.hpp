#ifndef INCLUDE_NXI_COMMAND_EXECUTOR_HPP_NXI
#define INCLUDE_NXI_COMMAND_EXECUTOR_HPP_NXI

#include <nds/graph/node.hpp>
#include <nxi/command.hpp>
#include <nxi/command/parameter.hpp>
#include <nxi/log.hpp>
#include <nxi/value.hpp>
#include <nxi/values.hpp>

#include <optional>

#include <QObject>

namespace nxi
{
    class command_executor : public QObject
    {
        Q_OBJECT
    public:
        command_executor(nds::node_ptr<const nxi::command> command)
            : complete_{ false }
            , command_{ command }
            , parameter_index_{ 0 }
        {}

        void add_value(const QString& value)
        {
            values_.add(value);
        }

        const nxi::command_parameter& active_parameter() const
        {
            return command_->parameter(parameter_index_);
        }

        bool is_ready() const
        {
            return parameter_index_ >= command_->parameters_count();
        }

        bool is_complete() const { return complete_; }

        void exec()
        {
            parameter_index_++;

            qDebug() << "param " << parameter_index_ << " " << complete_;
            if (complete_)
            {
                nxi_warning("command already executed");
                return;
            }
            if (is_ready())
            {
                command_->exec(values_);
                complete_ = true;
            }
        }

    signals:
        void event_command_complete();

    private:
        nds::node_ptr<const nxi::command> command_;
        bool complete_;
        unsigned int parameter_index_;
        nxi::values values_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_EXECUTOR_HPP_NXI