#ifndef UI_VIEW_RENDERER_H_NXI
#define UI_VIEW_RENDERER_H_NXI

#include <nxi/system/page.hpp>
#include <stz/observer_ptr.hpp>

#include <QWidget>

namespace nxw { class vbox_layout; }
namespace nxi { class page; }

namespace ui
{
    class user_session;
    class page;
    class renderer;

    class renderer_view : public QWidget
    {
        Q_OBJECT
    public:
        renderer_view(ui::user_session&, QWidget* parent);

        void display(const nxi::page&);
        void display(const nxi::page_system::pages_view pages);

    private:
        ui::user_session& session_;
        std::vector<stz::observer_ptr<ui::renderer>> renderers_;

        nxw::vbox_layout* layout_;
    };
} // ui

#endif // UI_VIEW_RENDERER_H_NXI