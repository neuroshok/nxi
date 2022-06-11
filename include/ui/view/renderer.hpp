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
    class buffer;
    class page;
}

class QLabel;

namespace ui
{
    class user;
    class page;
    class renderer;

    class renderer_view : public ui::basic_element<QWidget>
    {
        Q_OBJECT
    public:
        renderer_view(ui::user&, QWidget* parent);

        void display(nxi::page_system::page_ptr);
        void display(nxi::page_system::pages_view pages);

        [[nodiscard]] nxi::buffer& buffer() const { return buffer_; }

        void focusInEvent(QFocusEvent*) override;
        void focusOutEvent(QFocusEvent*) override;

    private:
        ui::user& user_;
        ui::renderer* renderer_;
        nxi::buffer& buffer_;

        QLabel* focus_marker_;
        nxw::vbox_layout* layout_;
    };
} // ui

#endif // UI_VIEW_RENDERER_H_NXI