#include <nxi/system/navigation.hpp>

#include <nds/graph.hpp>
#include <nds/graph/node.hpp>

#include <nxi/data/navigation.hpp>
#include <nxi/page.hpp>
#include <nxi/system/page.hpp>
#include <nxi/user_session.hpp>

namespace nxi
{
    navigation_system::navigation_system(nxi::user_session& session)
        : session_{ session }
        , recent_logs_cursor_{ 0 }
        , recent_logs_capacity_{ 3 + 1 }
        , load_source_{ load_source::standard }
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
        // we don't log page command change from navigation
        if (load_source_ != load_source::standard)
        {
            load_source_ = load_source::standard;
            return;
        }

        nxi::data::navigation::log(session_.database(), source.id(), source.command(), target_command, 0);
        recent_logs_.push_front(source.command());
        if (recent_logs_.size() > recent_logs_capacity_)  recent_logs_.pop_back();

        int i = 0;
        for (auto& item : recent_logs_)
        {
            QString c = "";
            if (i == recent_logs_cursor_) c = "___";
            qDebug() << c << item;
            ++i;
        }
    }

    void navigation_system::next_page_command()
    {
        if (recent_logs_cursor_ == 0)
        {
            nxi_warning("no more next {}", recent_logs_cursor_);
            return;
        }

        --recent_logs_cursor_;

        load_source_ = load_source::from_next;
        session_.page_system().focus()->load(get_page(recent_logs_cursor_));

                int i = 0;
        for (auto& item : recent_logs_)
        {
            QString c = "";
            if (i == recent_logs_cursor_) c = "___";
            qDebug() << c << item;
            ++i;
        }
    }

    void navigation_system::previous_page_command()
    {
        ++recent_logs_cursor_;
        if (recent_logs_cursor_ >= recent_logs_capacity_)
        {
            --recent_logs_cursor_;
            nxi_warning("no more previous {}", recent_logs_cursor_);
            return;
        }

        load_source_ = load_source::from_previous;
        session_.page_system().focus()->load(get_page(recent_logs_cursor_));

                int i = 0;
        for (auto& item : recent_logs_)
        {
            QString c = "";
            if (i == recent_logs_cursor_) c = "___";
            qDebug() << c << item;
            ++i;
        }
    }
    const QString& navigation_system::get_page(unsigned int index)
    {
        if (index >= recent_logs_.size()) nxi_warning("get_page index {}", index);
        return recent_logs_[recent_logs_cursor_];
    }

    const std::deque<QString>& navigation_system::page_command_logs() const
    {
        return recent_logs_;
    }
} // nxi