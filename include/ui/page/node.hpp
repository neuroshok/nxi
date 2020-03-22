#ifndef INCLUDE_UI_PAGE_NODE_HPP_NXI
#define INCLUDE_UI_PAGE_NODE_HPP_NXI

#include <ui/page.hpp>

namespace nxi { class page_node; }

namespace ui
{
    class core;
    class renderer;

    class node_page : public ui::page
    {
        Q_OBJECT
    public:
        node_page(ui::core& ui_core, nxi::page_node& page);

        void display(ui::renderer* renderer) override;
        ui::renderer* make_renderer() const override;

        QWidget* widget();

    private:
        ui::core& ui_core_;
        nxi::page_node& page_;
    };
} // ui

#endif // INCLUDE_UI_PAGE_NODE_HPP_NXI
