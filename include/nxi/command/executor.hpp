#ifndef INCLUDE_NXI_COMMAND_EXECUTOR_HPP_NXI
#define INCLUDE_NXI_COMMAND_EXECUTOR_HPP_NXI

#include <nds/graph/node.hpp>
#include <nxi/values.hpp>

#include <QObject>

class QString;

namespace nxi
{
    class command;
    class command_parameter;

    class command_executor : public QObject
    {
        Q_OBJECT
    public:
        command_executor(nds::node_ptr<const nxi::command> command);

        void add_value(const QString& value);
        void exec();
        void reset();

        const nxi::command_parameter& active_parameter() const;
        const nxi::command& command() const;
        bool is_complete() const;
        bool is_ready() const;

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