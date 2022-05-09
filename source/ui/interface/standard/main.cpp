#include <ui/interface/standard/main.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <nxw/hbox_layout.hpp>
#include <nxw/vbox_layout.hpp>

#include <ui/core.hpp>
#include <ui/interface/standard/content.hpp>
#include <ui/interface/standard/control_bar.hpp>
#include <ui/interface/standard/page_bar.hpp>

#include <ui/view/config.hpp>
#include <ui/view/page_tree.hpp>

#include <QMessageBox>
#include <nxi/system/page.hpp>
#include <nxi/type.hpp>

#include <QBrush>
#include <QImage>
#include <QPalette>
#include <QtGui/QPainter>
#include <ui/command/input.hpp>
#include <ui/command/menu.hpp>

namespace ui::interfaces::standard
{
    main::main(ui::user_session& session, ui::window* window)
        : ui::main_interface{ window }
        , session_{ session }
    {
        connect(&session_.nxi_session().interface_system(), &nxi::interface_system::event_update_style, [this](const nxi::style& style)
        {
            style.update(this);
        });


        auto main_layout = new nxw::vbox_layout;
        setLayout(main_layout);

        auto top_layout = new nxw::hbox_layout(this);
        auto middle_layout = new nxw::hbox_layout(this);

        content_ = new content(session_, window);
        //control_bar_ = new control_bar(session_, window);
        //page_bar_ = new page_bar(session_, window);

        static_cast<ui::window*>(this->window())->set_grip(this);

        top_layout->addWidget(control_bar_);
        middle_layout->addWidget(page_bar_);
        middle_layout->addWidget(content_);

        main_layout->addLayout(top_layout);
        main_layout->addLayout(middle_layout);
/*
        connect(&session_.nxi_session()., &nxi::core::event_error, this, [](const QString& message)
        {
            auto* error = new QMessageBox;
            error->setAttribute(Qt::WA_DeleteOnClose, true);
            error->setWindowTitle("nxi error");
            error->setText(message);
            error->show();
        });*/
    }

    bool main::toggle_fullmode(int state)
    {
        nxi_debug("fullmode");

        main_interface::toggle_fullmode();
        if (main_interface::fullmode())
        {
            control_bar_->hide();
            page_bar_->hide();
        }
        else
        {
            control_bar_->show();
            page_bar_->show();
        }
        return true;
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

    void main::resizeEvent(QResizeEvent*)
    {

    }
} // ui::interfaces