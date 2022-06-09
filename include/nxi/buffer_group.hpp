#ifndef INCLUDE_NXI_BUFFER_GROUP_HPP_NXI
#define INCLUDE_NXI_BUFFER_GROUP_HPP_NXI

#include <nxi/suggestion/vector.hpp>

#include <QObject>

namespace nxi
{
    class buffer;

    class buffer_group : public QObject
    {
        Q_OBJECT
    public:
        explicit buffer_group(nxi::core&, int id);

        void exec();

        void set_command_root(nds::node_ptr<nxi::command>);
        void set_page_root(nds::node_ptr<nxi::page>);

        void context_suggest();

        void suggest_command();
        void suggest_context();
        void suggest_navigation();
        void suggest_page();
        void suggest_session();

        [[nodiscard]] int id() const;
        [[nodiscard]] nds::node_ptr<nxi::command> command_root() const;
        [[nodiscard]] nds::node_ptr<nxi::page> page_root() const;
        [[nodiscard]] nxi::suggestion_vector& suggestions();
        [[nodiscard]] const nxi::suggestion_vector& suggestions() const;

    signals:
        void event_page_activate(nds::node_ptr<nxi::page>);
        void event_buffer_focus(nxi::buffer&);
        void event_action_update(const nxi::suggestion_vector&);
        void event_page_root_update(nds::node_ptr<nxi::page>);
        void event_command_root_update(nds::node_ptr<nxi::command>);

    private:
        nxi::core& core_;
        int id_;
        nds::node_ptr<nxi::command> command_root_;
        nds::node_ptr<nxi::page> page_root_;
        nxi::suggestion_vector suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_BUFFER_GROUP_HPP_NXI