#ifndef UI_RENDERER_VIEW_H_NXI
#define UI_RENDERER_VIEW_H_NXI

#include <QWidget>
#include <nxi/system/page.hpp>

namespace nxw { class vbox_layout; }
namespace nxi { class page; }

namespace ui
{
    class core;
    class page;
    class renderer;

    class renderer_view : public QWidget
    {
        Q_OBJECT
    public:
        renderer_view(ui::core& ui_core, QWidget* parent);


        void display(nxi::page& page);
        void display(nxi::page_system::pages_view pages);

    private:
        ui::core& ui_core_;
        std::vector<renderer*> renderers_;
        renderer* renderer_;

        nxw::vbox_layout* layout_;
    };
} // ui

#endif // UI_RENDERER_VIEW_H_NXI