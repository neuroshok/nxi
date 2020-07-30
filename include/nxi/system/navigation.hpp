#ifndef INCLUDE_NXI_SYSTEM_NAVIGATION_HPP_NXI
#define INCLUDE_NXI_SYSTEM_NAVIGATION_HPP_NXI

#include <nds/graph.hpp>
#include <nds/graph/node.hpp>

#include <deque>
#include <QObject>

namespace nxi
{
    class page;
    class session;

    class navigation_system : public QObject
    {
    public:
        navigation_system(nxi::session&);
        navigation_system(const navigation_system&) = delete;
        void operator=(const navigation_system&) = delete;

        void load();

        void log_page_command(const nxi::page& source, const QString& target);

        void next_page_command();
        void previous_page_command();

        const QString& get_page(unsigned int index);

        template<class Context>
        void next()
        {
            /*
            session_.nxi_session().context_system().apply_on_focus
            (
                [this, &items](const nxi::contexts::command&)
                {

                }
                , [this, &items](const nxi::contexts::page&)
                {

                }
            );*/
        }

        void previous();

    private:
        nxi::session& session_;

        unsigned int recent_logs_cursor_;
        unsigned int recent_logs_capacity_;

        std::deque<QString> recent_logs_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_NAVIGATION_HPP_NXI