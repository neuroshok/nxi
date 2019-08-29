#ifndef UI_CONTENT_H_NXI
#define UI_CONTENT_H_NXI

#include <ui/interface.hpp>
#include <ui/view/renderer.hpp>

namespace nxw
{
    class web_view;
    class explorer_view;
} // nxw

namespace ui { class core; class web_view; }

class QStackedWidget;

namespace ui::interfaces
{
    class content : public ui::interface
    {
    public:

        explicit content(ui::core& ui_core);

    private:
        ui::core& ui_core_;

        ui::renderer_view* renderer_view_;
    };
} // ui::interfaces

#endif // UI_CONTENT_H_NXI