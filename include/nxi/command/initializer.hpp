#ifndef INCLUDE_NXI_COMMAND_INITIALIZER_HPP_NXI
#define INCLUDE_NXI_COMMAND_INITIALIZER_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/command/data.hpp>

namespace nds
{
    template<class> class node;
} // nds

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
        nds::node<nxi::command>* add(nxi::command_data data);
        nds::node<nxi::command>* add(const QString& action, nxi::command_function_type fn, const QString& icon = "");
        nds::node<nxi::command>* add_node(const QString& command_node);
        void add_param(const QString& name, std::function<void(nxi::suggestion_vector&)> fn);

        void init_main();
        void init_page();

        void set_root(nds::node<nxi::command>*);

    private:
        nxi::core& nxi_core_;
        nds::node<nxi::command>* command_;
        nds::node<nxi::command>* node_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_INITIALIZER_HPP_NXI
