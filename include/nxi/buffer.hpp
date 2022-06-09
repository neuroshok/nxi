#ifndef INCLUDE_NXI_BUFFER_HPP_NXI
#define INCLUDE_NXI_BUFFER_HPP_NXI

#include <nds/graph/node.hpp>

#include <nxi/command/input.hpp>
#include <nxi/page.hpp>

#include <QObject>

namespace nxi
{
    class core;
    class buffer_group;

    class buffer : public QObject
    {
        Q_OBJECT
    public:
        buffer(nxi::core& core, nxi::buffer_group& group, int id);

        void set_page(nds::node_ptr<nxi::page>);

        [[nodiscard]] int id() const;
        [[nodiscard]] nxi::buffer_group& group();
        [[nodiscard]] nds::node_ptr<nxi::page> page();
        [[nodiscard]] nxi::command_input& input();

    private:
        nxi::core& core_;

        int id_;
        nxi::buffer_group& group_;
        nxi::command_input input_;
        nds::node_ptr<nxi::page> page_;
    };
} // nxi

#endif // INCLUDE_NXI_BUFFER_HPP_NXI