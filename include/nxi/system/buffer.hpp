#ifndef INCLUDE_NXI_SYSTEM_BUFFER_HPP_NXI
#define INCLUDE_NXI_SYSTEM_BUFFER_HPP_NXI

#include <nxi/command/input.hpp>

#include <memory>
#include <vector>

#include <QObject>

namespace nxi
{
    class core;

    class buffer : public QObject
    {
        Q_OBJECT
    public:
        buffer(nxi::core& core, int id)
            : id_{ id }
            , input_{ core }
        {}

        int id() const { return id_; }
        nds::node_ptr<nxi::page> page() { return page_; }
        nxi::command_input& input() { return input_; }

        void set_page(nds::node_ptr<nxi::page> page) { page_ = page; }

    private:
        int id_;
        nxi::command_input input_;
        nds::node_ptr<nxi::page> page_;
    };

    class buffer_system : public QObject
    {
        Q_OBJECT
    public:
        explicit buffer_system(nxi::core& core);

        nxi::buffer& get(int id);
        void focus(int id);
        nxi::buffer& focus();

        int add();

    signals:
        void event_focus(nxi::buffer&);

    private:
        nxi::core& core_;
        int focus_;
        std::vector<std::unique_ptr<nxi::buffer>> buffers_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_BUFFER_HPP_NXI