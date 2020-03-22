#include <ui/page/node.hpp>

#include <nxi/page/node.hpp>
#include <nxi/core.hpp>

#include <ui/core.hpp>
#include <ui/renderer/widget.hpp>
#include <nxw/hbox_layout.hpp>

#include <QLabel>
#include <QPushButton>

namespace ui
{
    node_page::node_page(ui::core& ui_core, nxi::page_node& page)
        : ui::page{ page }
        , page_{ page }
        , ui_core_{ ui_core }
    {
        auto* main_layout = new nxw::hbox_layout;
        setLayout(main_layout);

        // ui_core_.nxi_core().page_system().list(page.id())
        auto l = new QLabel(this);
        auto ll = new QPushButton(this);
        l->setText("prout");
        ll->setText("proutproutprout");

        //main_layout->addWidget(l);
        main_layout->addWidget(ll);
    }

    void node_page::display(ui::renderer *renderer)
    {
        renderer->display(this);
    }

    ui::renderer* node_page::make_renderer() const
    {
        return new widget_renderer;
    }

    QWidget* node_page::widget() { return this; }
} // ui