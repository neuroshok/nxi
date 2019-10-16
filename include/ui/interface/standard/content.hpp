#ifndef INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI
#define INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI

#include <ui/interface.hpp>
#include <ui/view/renderer.hpp>

namespace nxw
{
    class web_view;
    class explorer_view;
} // nxw

namespace ui { class core; class web_view; }

class QStackedWidget;

namespace ui::interfaces::standard
{
    class content : public ui::interface
    {
    public:

        explicit content(ui::core& ui_core, ui::window* window);

    private:
        ui::core& ui_core_;

        ui::renderer_view* renderer_view_;
    };
} // ui::interfaces::standard

#endif // INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI