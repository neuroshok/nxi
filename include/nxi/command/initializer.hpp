#ifndef INCLUDE_NXI_COMMAND_INITIALIZER_HPP_NXI
#define INCLUDE_NXI_COMMAND_INITIALIZER_HPP_NXI

#include <nxi/command/data.hpp>
#include <nxi/command/fwd.hpp>

#include <nds/graph/node.hpp>

class QString;

namespace nxi
{
    class core;
    class suggestion_vector;

    class command_initializer
    {
    public:
        command_initializer(nxi::core& nxi_core);
        void load();

    private:
        nds::node_ptr<nxi::command> add(nxi::command_data data);
        nds::node_ptr<nxi::command> add(const QString& action, nxi::command_function_type fn, const QString& icon = "");
        nds::node_ptr<nxi::command> add_node(const QString& command_node);

        void init_main();
        void init_page();

        void set_root(nds::node_ptr<nxi::command>);

    private:
        nxi::core& nxi_core_;
        nds::node_ptr<nxi::command> command_;
        nds::node_ptr<nxi::command> node_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_INITIALIZER_HPP_NXI
