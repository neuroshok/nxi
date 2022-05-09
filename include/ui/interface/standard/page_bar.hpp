#ifndef INCLUDE_UI_INTERFACE_STANDARD_PAGE_BAR_HPP_NXI
#define INCLUDE_UI_INTERFACE_STANDARD_PAGE_BAR_HPP_NXI

#include <ui/window.hpp>
#include <ui/interface.hpp>
#include <ui/view/page_tree/page_item.hpp>
#include <ui/view/page_tree.hpp>

namespace nxw { class tree; };
namespace nxi { class web_page; class page_node; };

namespace ui::interfaces::standard
{
    class page_bar : public ui::interface
    {
        Q_OBJECT
    public:

        explicit page_bar(ui::user_session&, ui::window*);

    private:
        ui::user_session& session_;
        ui::views::page_tree* page_tree_;
    };
} // ui::interfaces::standard

#endif // INCLUDE_UI_INTERFACE_STANDARD_PAGE_BAR_HPP_NXI