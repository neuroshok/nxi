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
        , main_group_{ core_, 0 }
        , main_buffer_{ core_, main_group_, 0 }
        , focus_{ &main_buffer_ }
    {}

    void buffer_system::load()
    {
        nxi_trace("");

        main_group_.set_command_root(core_.command_system().root());
        main_group_.set_page_root(core_.page_system().root());

        for (auto& group : groups_)
            group->load();

        // focus the main buffer
        emit event_focus(main_buffer_);
        emit event_focus_changed(main_buffer_, main_buffer_);
        emit main_group_.event_buffer_focus(main_buffer_);
    }

    void buffer_system::focus(nxi::buffer& buffer)
    {
        nxi_trace("focus buffer {}", buffer.id());
        auto previous_focus = focus_;
        focus_ = &buffer;

        emit event_focus(buffer);

        if (previous_focus != focus_)
        {
            emit event_focus_changed(*previous_focus, buffer);
        }

        emit buffer.group().event_buffer_focus(buffer);
    }

    nxi::buffer_group& buffer_system::add_group(int group_id)
    {
        auto v_group = std::make_unique<nxi::buffer_group>(core_, group_id);
        v_group->load();
        groups_.emplace_back(std::move(v_group));

        return *groups_.back();
    }

    void buffer_system::del(int buffer_id)
    {
        nxi_trace("del buffer {} ", buffer_id);

        buffers_.erase(std::remove_if(buffers_.begin(), buffers_.end(), [buffer_id](const auto& buffer) { return buffer->id() == buffer_id; }),
                       buffers_.end());
    }

    void buffer_system::del_group(int group_id)
    {
        nxi_trace("del group {} ", group_id);

        groups_.erase(std::remove_if(groups_.begin(), groups_.end(), [group_id](const auto& group) { return group->id() == group_id; }),
                      groups_.end());

        buffers_.erase(std::remove_if(buffers_.begin(), buffers_.end(), [group_id](const auto& buffer) { return buffer->group().id() == group_id; }),
                       buffers_.end());
    }

    nxi::buffer& buffer_system::add(int group_id)
    {
        if (!group_exists(group_id))
        {
            nxi_warning("adding a buffer in an inexistent group {}, it will be created", group_id);
            add_group(group_id);
        }
        auto& v_group = group(group_id);

        int id = static_cast<int>(buffers_.size() + 1); // start index at 1, 0 is reserved for main buffer
        buffers_.emplace_back(std::make_unique<nxi::buffer>(core_, v_group, id));
        nxi_trace("add buffer {} in group {}", id, group_id);
        return *buffers_.back();
    }

    bool buffer_system::group_exists(int id) const
    {
        return std::find_if(groups_.begin(), groups_.end(), [id](const auto& group_ptr) { return group_ptr->id() == id; }) != groups_.end();
    }

    nxi::buffer& buffer_system::buffer() { return main_buffer_; }

    nxi::buffer& buffer_system::focus()
    {
        nxi_assert(focus_);
        return *focus_;
    }

    nxi::buffer_group& buffer_system::group() { return main_group_; }

    nxi::buffer_group& buffer_system::group(int id)
    {
        if (!group_exists(id))
        {
            nxi_warning("adding a buffer in an inexistent group {}, it will be created", id);
            add_group(id);
        }
        return **std::find_if(groups_.begin(), groups_.end(), [id](const auto& group) { return group->id() == id; });
    }
} // nxi