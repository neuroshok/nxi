#ifndef INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI
#define INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI

#include <ui/interface.hpp>
#include <ui/view/renderer.hpp>

namespace nxw
{
    class explorer_view;
} // nxw

namespace ui { class session; }

class QStackedWidget;

namespace ui::interfaces::standard
{
    class content : public ui::interface
    {
    public:

        explicit content(ui::session& session, ui::window* window);

    private:
        ui::session& session_;

        ui::renderer_view* renderer_view_;
    };
} // ui::interfaces::standard

#endif // INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI