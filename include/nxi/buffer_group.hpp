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
        explicit buffer_group(int id);

        [[nodiscard]] int id() const;
        [[nodiscard]] nxi::suggestion_vector& suggestions();

    signals:
        void event_activate_page(nds::node_ptr<nxi::page>);
        void event_buffer_focus(nxi::buffer&);
        void event_action_update(const nxi::suggestion_vector&);

    private:
        int id_;
        nxi::suggestion_vector suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_BUFFER_GROUP_HPP_NXI