#include <ui/interface/main.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <nxw/hbox_layout.hpp>
#include <nxw/vbox_layout.hpp>

#include <ui/core.hpp>
#include <ui/interface/content.hpp>
#include <ui/interface/control_bar.hpp>
#include <ui/interface/page_bar.hpp>

#include <ui/view/config.hpp>
#include <ui/view/page_tree.hpp>

#include <QMessageBox>

namespace ui::interfaces
{
    main::main(ui::core& ui_core)
        : ui::interface("main")
        , ui_core_{ ui_core }
    {
        auto main_layout = new nxw::vbox_layout;
        setLayout(main_layout);

        auto top_layout = new nxw::hbox_layout(this);
        auto middle_layout = new nxw::hbox_layout(this);

        content_ = new interfaces::content(ui_core_);
        control_bar_ = new ui::interfaces::control_bar(ui_core_);
        page_bar_ = new ui::interfaces::page_bar(ui_core_);

        top_layout->addWidget(control_bar_);

        middle_layout->addWidget(page_bar_);
        middle_layout->addWidget(content_);

        main_layout->addLayout(top_layout);
        main_layout->addLayout(middle_layout);

        connect(&ui_core_.nxi_core(), &nxi::core::event_error, this, [](const QString& message)
        {
            auto* error = new QMessageBox;
            error->setAttribute(Qt::WA_DeleteOnClose, true);
            error->setWindowTitle("nxi error");
            error->setText(message);
            error->show();
        });
    }

    void main::toggle_fullmode()
    {
        nxi_debug("fullmode");

        interface::toggle_fullmode();
        if (interface::fullmode())
        {
            control_bar_->hide();
            page_bar_->hide();
        }
        else
        {
            control_bar_->show();
            page_bar_->show();
        }
    }
} // ui::interfaces