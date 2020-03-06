#include <ui/interface/light/main.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <nxw/hbox_layout.hpp>
#include <nxw/vbox_layout.hpp>

#include <ui/core.hpp>
#include <ui/interface/standard/content.hpp>
#include <ui/interface/light/control_bar.hpp>


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
#include <ui/command/menu.hpp>
#include <ui/window.hpp>
#include <include/nxi/style_data.hpp>

namespace ui::interfaces::light
{
    main::main(ui::core& ui_core, ui::window* window)
        : ui_core_{ ui_core }
    {
        connect(&ui_core_.nxi_core().interface_system(), &nxi::interface_system::event_update_style, [this](const nxi::style& style)
        {
            style.update(this);
        });

        auto main_layout = new nxw::vbox_layout;
        setLayout(main_layout);

        auto top_layout = new nxw::hbox_layout(this);
        auto middle_layout = new nxw::hbox_layout(this);

        content_ = new interfaces::standard::content(ui_core_, window);
        control_bar_ = new ui::interfaces::light::control_bar(ui_core_, window);

        static_cast<ui::window*>(this->window())->set_grip(this);

        top_layout->addSpacing(120);
        top_layout->addWidget(control_bar_);
        top_layout->addSpacing(120);
        middle_layout->addWidget(content_);

        main_layout->addLayout(top_layout);
        main_layout->addLayout(middle_layout, 1);

        command_menu_ = new ui::command_menu(ui_core_, this);
        command_menu_->hide();

        setFocusPolicy(Qt::ClickFocus);

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_reset, [this]()
        {
            command_menu_->hide();
        });

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_suggestion_update, [this]
        (const stz::observer_ptr<const nxi::suggestion_vector> suggestions)
        {
            command_menu_->set_data(suggestions);
            command_menu_->exec();
        });

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
        main_interface::toggle_fullmode();
        if (main_interface::fullmode()) control_bar_->hide();
        else control_bar_->show();
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

        // insane design drawing
        auto& design_color = ui_core_.nxi_core().interface_system().style().data().field.background_color.get();
        painter.fillRect(0, 0 , width(), control_bar_->height(), design_color);
    }

    void main::resizeEvent(QResizeEvent*)
    {
        command_menu_->setFixedWidth(control_bar_->width());
        command_menu_->move(control_bar_->x(), control_bar_->height());
    }
} // ui::interfaces