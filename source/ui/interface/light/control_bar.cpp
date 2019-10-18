#include <ui/interface/light/control_bar.hpp>

#include <ui/core.hpp>
#include <nxi/core.hpp>
#include <nxi/system/command.hpp>

#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

#include <nxw/hbox_layout.hpp>
#include <nxw/icon_button.hpp>
#include <nxw/menu.hpp>
#include <nxw/menu/item.hpp>
#include <include/ui/interface/standard/control_bar.hpp>
#include <ui/view/web.hpp>
#include <QWebEngineView>
#include <include/nxi/module/web.hpp>
#include <include/nxi/log.hpp>
#include <nxi/config.hpp>

#include <ui/command.hpp>
#include <ui/window.hpp>

#include <QLabel>
#include <QPushButton>

namespace ui::interfaces::light
{
    control_bar::control_bar(ui::core& ui_core, ui::window* window)
        : ui::interface("control_bar", window)
        , ui_core_{ ui_core }
    {
        nxi_trace("");

        QHBoxLayout* layout = new nxw::hbox_layout;
        setLayout(layout);

        command_root_ = new QLabel(this);
        command_root_->setText("nxi");
        command_root_->setStyleSheet("font-weight: bold; background-color: #0F1419; color: #00BBFF; padding: 0 20 0 20;");

        command_input_ = new ui::command(ui_core_);
        command_input_->setFocus();
        layout->addWidget(command_root_);
        layout->addWidget(command_input_);

        connect(&ui_core_.nxi_core().command_system(), &nxi::command_system::event_root_update,
        [this](nds::node<nxi::command>* command)
        {
            command_root_->setText(command->get().action_name());
        });
    }

    ui::command *control_bar::command_input()
    {
        return command_input_;
    }
} // ui::interfaces