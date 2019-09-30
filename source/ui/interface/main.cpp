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
#include <nxi/system/page.hpp>
#include <nxi/type.hpp>

#include <QBrush>
#include <QImage>
#include <QPalette>
#include <QtGui/QPainter>
#include <ui/command.hpp>

namespace ui::interfaces
{
    main::main(ui::core& ui_core, ui::window* window)
        : ui::interface("main", window)
        , ui_core_{ ui_core }
    {
        connect(&ui_core_.nxi_core().interface_system(), &nxi::interface_system::event_update_style, [this](const nxi::style& style)
        {
            style.update(this);
        });

        auto main_layout = new nxw::vbox_layout;
        setLayout(main_layout);

        auto top_layout = new nxw::hbox_layout(this);
        auto middle_layout = new nxw::hbox_layout(this);

        content_ = new interfaces::content(ui_core_, window);
        //control_bar_ = new ui::interfaces::control_bar(ui_core_, window);
        //page_bar_ = new ui::interfaces::page_bar(ui_core_, window);

        static_cast<ui::window*>(this->window())->set_grip(this);

        command_bar_ = new ui::command(ui_core_);
        top_layout->addSpacing(128);
        top_layout->addWidget(command_bar_);
        top_layout->addSpacing(128);
        //middle_layout->addWidget(page_bar_);
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
            //control_bar_->hide();
            //page_bar_->hide();
            command_bar_->hide();
        }
        else
        {
            //control_bar_->show();
            //page_bar_->show();
            command_bar_->show();
        }
    }

    void main::paintEvent(QPaintEvent*)
    {
        auto& image = style_data.background_image;

        auto d_y = image.height() - height();

        QRectF source(0.0, 0.0, image.width(), image.height());
        QRectF target(0, -d_y, image.width(), image.height());

        QPainter painter(this);
        painter.fillRect(0, 0 ,width(), height(), style_data.background_color);
        painter.drawImage(target, image, source);
    }
} // ui::interfaces