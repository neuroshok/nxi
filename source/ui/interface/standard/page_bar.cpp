#include <ui/interface/standard/page_bar.hpp>

#include <ui/core.hpp>
#include <nxi/core.hpp>

#include <nxw/hbox_layout.hpp>
#include <ui/view/page_tree.hpp>
#include <nxw/vbox_layout.hpp>

#include <ui/menu.hpp>
#include <nxw/icon_button.hpp>
#include <include/ui/interface/standard/page_bar.hpp>
#include <include/nxi/log.hpp>
#include <nxi/page/web.hpp>


namespace ui::interfaces::standard
{
    page_bar::page_bar(ui::core& ui_core, ui::window* window) :
        ui::interface("page_bar", window)
        , ui_core_{ ui_core }
    {
        /*
        connect(&ui_core_.nxi_core().interface_system(), &nxi::interface_system::event_update_style, [this](const nxi::style& style){
            style.update(this);
            style.update(page_tree_);
        });*/

        if (ui_core_.nxi_core().config().browser.interface.show_page_bar.get()) show();
        //else hide();

        auto layout = new nxw::vbox_layout;
        setLayout(layout);

        setFixedWidth(200);

        auto page_control_layout = new nxw::hbox_layout;

        page_tree_ = new ui::views::page_tree(ui_core_);

        connect(&ui_core_.nxi_core().page_system(), &nxi::page_system::event_add, [this](nxi::page_system::page_ptr page, nxi::page_system::page_ptr)
        {
            //page_tree_->add(*page, source_id);
        });

        auto btn_web_new = new nxw::icon_button(this, ":/button/page_web_new");
        auto btn_explorer_new = new nxw::icon_button(this, ":/button/page_explorer_new");

        page_control_layout->addWidget(btn_web_new);
        page_control_layout->addWidget(btn_explorer_new);
        page_control_layout->addStretch(1);

        layout->addLayout(page_control_layout);
        layout->addWidget(page_tree_);

        connect(btn_web_new, &QPushButton::clicked, this, [this]()
        {
            ui_core_.nxi_core().page_system().add<nxi::web_page>();
        });

        connect(btn_explorer_new, &QPushButton::clicked, this, [this]()
        {
            //ui_core_.nxi_core().page_system().add(nxi::explorer_page{}, 0);
        });
    }
} // ui::interfaces