#include <nxi/system/navigation.hpp>

#include <nds/graph.hpp>
#include <nds/graph/node.hpp>

#include <nxi/data/navigation.hpp>
#include <nxi/page.hpp>
#include <nxi/session.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    navigation_system::navigation_system(nxi::session& session)
        : session_{ session }
        , recent_logs_cursor_{ 0 }
        , recent_logs_capacity_{ 2 }
    {}

    void navigation_system::load()
    {
        connect(&session_.page_system(), &nxi::page_system::event_update_command, [this](const nxi::page& source, const QString& target)
        {
            log_page_command(source, target);
        });
    }

    void navigation_system::log_page_command(const nxi::page& source, const QString& target_command)
    {
        nxi::data::navigation::log(session_.database(), source.id(), source.command(), target_command, 0);
        recent_logs_.push_back(target_command);
        if (recent_logs_.size() > recent_logs_capacity_)  recent_logs_.pop_front();
    }

    void navigation_system::next_page_command()
    {
        if (recent_logs_cursor_ == 0)
        {
            nxi_warning("no more next {}", recent_logs_cursor_);
            return;
        }
        --recent_logs_cursor_;
        session_.page_system().focus()->update_command(get_page(recent_logs_cursor_));
    }

    void navigation_system::previous_page_command()
    {
        if (recent_logs_cursor_ >= recent_logs_capacity_)
        {
            --recent_logs_cursor_;
            nxi_warning("no more previous {}", recent_logs_cursor_);
            return;
        }
        session_.page_system().focus()->update_command(get_page(recent_logs_cursor_));
        ++recent_logs_cursor_;
    }
    const QString& navigation_system::get_page(unsigned int index)
    {
        if (index >= recent_logs_.size()) nxi_warning("get_page index {}", index);
        return recent_logs_[recent_logs_cursor_];
    }
} // nxi