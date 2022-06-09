#include <nxi/system/buffer.hpp>

#include <nxi/core.hpp>
#include <nxi/system/command.hpp>
/*!
 * \description we create a default buffer group and buffer to fallback on it when there is no buffer created by the ui
 * so we can always have a focus
 */

namespace nxi
{
    buffer_system::buffer_system(nxi::core& core)
        : core_{ core }
        , default_group_{ core_, 0 }
        , default_buffer_{ core_, default_group_, 0 }
        , focus_{ &default_buffer_ }
    {}

    void buffer_system::load()
    {
        nxi_trace("");

        default_group_.set_command_root(core_.command_system().root());
        default_group_.set_page_root(core_.page_system().root());
        for (auto& group : groups_)
        {
            group->set_command_root(core_.command_system().root());
            group->set_page_root(core_.page_system().root());
        }
    }

    void buffer_system::focus(nxi::buffer& buffer)
    {
        focus_ = &buffer;

        emit event_focus(buffer);
    }

    nxi::buffer_group& buffer_system::add_group(int group_id)
    {
        auto v_group = std::make_unique<nxi::buffer_group>(core_, group_id);
        groups_.emplace_back(std::move(v_group));

        return *groups_.back();
    }

    nxi::buffer& buffer_system::add(int group_id)
    {
        if (!group_exists(group_id)) add_group(group_id);
        auto& v_group = group(group_id);

        int id = static_cast<int>(buffers_.size());
        buffers_.emplace_back(std::make_unique<nxi::buffer>(core_, v_group, id));
        nxi_trace("add buffer {}", id);
        focus(*buffers_.back());
        return *buffers_.back();
    }

    bool buffer_system::group_exists(int id) const
    {
        return std::find_if(groups_.begin(), groups_.end(), [id](const auto& group_ptr) { return group_ptr->id() == id; }) != groups_.end();
    }

    nxi::buffer& buffer_system::buffer() { return default_buffer_; }

    nxi::buffer& buffer_system::focus()
    {
        nxi_assert(focus_);
        return *focus_;
    }

    nxi::buffer_group& buffer_system::group() { return default_group_; }

    nxi::buffer_group& buffer_system::group(int id)
    {
        // nxi_assert(group_exists(id));
        if (!group_exists(id)) add_group(id);
        return **std::find_if(groups_.begin(), groups_.end(), [id](const auto& group) { return group->id() == id; });
    }
} // nxi