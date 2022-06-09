#ifndef INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI
#define INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI

#include <ui/interface.hpp>
#include <ui/view/renderer.hpp>

namespace nxw
{
    class hbox_layout;
} // nxw

namespace ui { class user_session; }

namespace ui::interfaces::standard
{
    class content : public ui::interface
    {
    public:
        explicit content(ui::user_session& session, ui::window* window);

        ui::renderer_view* add();

    private:
        ui::user_session& session_;

        nxw::hbox_layout* layout_;
        ui::renderer_view* focus_;

        inline static std::unordered_map<int, ui::renderer_view*> views_{};
    };
} // ui::interfaces::standard

#endif // INCLUDE_UI_INTERFACE_STANDARD_CONTENT_HPP_NXI