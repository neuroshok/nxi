#include <nxi/buffer.hpp>

#include <nxi/buffer_group.hpp>
#include <nxi/core.hpp>

namespace nxi
{
    buffer::buffer(nxi::core& core, nxi::buffer_group& group, int id)
        : core_{ core }
        , id_{ id }
        , group_{ group }
        , input_{ core }
    {}

    void buffer::set_page(nds::node_ptr<nxi::page> page) { page_ = page; }

    [[nodiscard]] int buffer::id() const { return id_; }
    [[nodiscard]] const nxi::buffer_group& buffer::group() const { return group_; }
    [[nodiscard]] nds::node_ptr<nxi::page> buffer::page() { return page_; }
    [[nodiscard]] nxi::command_input& buffer::input() { return input_; }
} // nxi