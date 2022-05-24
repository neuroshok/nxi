#ifndef UI_VIEW_RENDERER_H_NXI
#define UI_VIEW_RENDERER_H_NXI

#include <nxi/system/page.hpp>
#include <stz/observer_ptr.hpp>
#include <ui/element.hpp>

#include <QWidget>

namespace nxw
{
    class vbox_layout;
}
namespace nxi
{
    class page;
}

class QLabel;

namespace ui
{
    class user_session;
    class page;
    class renderer;

    class renderer_view : public ui::basic_element<QWidget>
    {
        Q_OBJECT
    public:
        renderer_view(ui::user_session&, QWidget* parent);

        void display(nxi::page_system::page_ptr);
        void display(nxi::page_system::pages_view pages);

        int buffer_id() const { return buffer_id_; }

        void focusInEvent(QFocusEvent*) override;
        void focusOutEvent(QFocusEvent*) override;

    private:
        ui::user_session& session_;
        ui::renderer* renderer_;
        int buffer_id_;

        QLabel* focus_marker_;
        nxw::vbox_layout* layout_;
    };
} // ui

#endif // UI_VIEW_RENDERER_H_NXI