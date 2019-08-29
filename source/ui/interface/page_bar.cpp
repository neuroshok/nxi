#include <ui/interface/page_bar.hpp>

#include <ui/core.hpp>
#include <nxi/core.hpp>

#include <nxw/hbox_layout.hpp>
#include <ui/view/page_tree.hpp>
#include <nxw/vbox_layout.hpp>

#include <nxw/menu.hpp>
#include <nxw/icon_button.hpp>
#include <include/ui/interface/page_bar.hpp>
#include <include/nxi/log.hpp>


namespace ui::interfaces
{
    page_bar::page_bar(ui::core& ui_core) :
        ui_core_{ ui_core }
    {
        if (ui_core_.nxi_core().config().browser.interface.show_page_bar.get()) show();
        //else hide();

        auto layout = new nxw::vbox_layout;
        setLayout(layout);

        setFixedWidth(200);

        auto page_control_layout = new nxw::hbox_layout;

        page_tree_ = new ui::page_tree_view(ui_core_);

        connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&, nxi::page_id>(&nxi::page_system::event_add), [this](nxi::page& page,  nxi::page_id source_id)
        {
            nxi_trace("page_tree add page {} renderer {}", page.name(), (int)page.renderer_type());
            page_tree_->add(page, source_id);
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