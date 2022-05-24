#include <nxi/system/buffer.hpp>

namespace nxi
{
    buffer_system::buffer_system(nxi::core& core)
        : core_{ core }
        , focus_{ 0 }
    {}

    nxi::buffer& buffer_system::get(int id)
    {
        nxi_assert(id >= 0 && id < buffers_.size());
        return *buffers_[id];
    }
    void buffer_system::focus(int id)
    {
        focus_ = id;

        emit event_focus(get(id));
    }

    int buffer_system::add()
    {
        int id = static_cast<int>(buffers_.size());
        buffers_.emplace_back(std::make_unique<nxi::buffer>(core_, id));
        nxi_trace("add buffer {}", id);
        return id;
    }
    nxi::buffer& buffer_system::focus() { return get(focus_); }
} // nxi