#include <ui/interface/light/main.hpp>

#include <nxi/core.hpp>
#include <nxi/notification_data.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/session.hpp>

#include <nxw/hbox_layout.hpp>
#include <nxw/vbox_layout.hpp>

#include <ui/core.hpp>
#include <ui/interface/light/control_bar.hpp>
#include <ui/interface/standard/content.hpp>

#include <ui/command/input.hpp>
#include <ui/command/menu.hpp>
#include <ui/interface/standard/window_control.hpp>
#include <ui/notification.hpp>
#include <ui/system/user.hpp>
#include <ui/view/config.hpp>
#include <ui/view/page_tree.hpp>
#include <ui/window.hpp>

#include <QImage>
#include <QMessageBox>
#include <QProgressBar>
#include <QWebEngineView>
#include <QtGui/QPainter>

namespace ui::interfaces::light
{
    main::main(ui::user_session& session, ui::window* window)
        : ui::main_interface{ window }
        , session_{ session }
    {
        connect(&session_.nxi_session().interface_system(), &nxi::interface_system::event_update_style, [this](const nxi::style& style) { style.update(this); });

        connect(&session_.nxi_session().session_system(), &nxi::session_system::event_focus, [this](const nxi::session& session) {
            auto s = session.config().browser.interface.style.get();
            session_.nxi_session().interface_system().load_style(s);
        });

        auto main_layout = new nxw::vbox_layout;
        setLayout(main_layout);

        auto top_layout = new nxw::hbox_layout(this);
        auto middle_layout = new nxw::hbox_layout(this);

        content_ = new interfaces::standard::content(session_, window);
        control_bar_ = new ui::interfaces::light::control_bar(session_, window);

        auto window_control = new ui::interfaces::standard::window_control(session_, window);

        static_cast<ui::window*>(this->window())->set_grip(this);
        auto bar = new QProgressBar{ this };
        bar->hide();

        top_layout->addSpacing(120);
        top_layout->addWidget(control_bar_);
        middle_layout->addWidget(content_);

        main_layout->addLayout(top_layout);
        main_layout->addLayout(middle_layout, 1);
        main_layout->addWidget(bar);

        command_menu_ = new ui::command_menu(session_, this);
        command_menu_->hide();

        setFocusPolicy(Qt::ClickFocus);

        auto session_info_ = new QLabel{ this };

        connect(&session_.nxi_session().nxi_core(), &nxi::core::event_load, [bar, this]() {
            connect(&session_.nxi_core().session().web_downloader(), &nxi::web_downloader::event_update, [bar](float percent) {
                if (percent > 99) bar->hide();
                else bar->show();
                bar->setValue(percent);
            });
            connect(&session_.nxi_core().notification_system(), &nxi::notification_system::event_send, [this](const nxi::notification_data& data) {
                auto notification = ui::notification::make(data, session_.nxi_core().session_config().browser.notification_mode.get());
                notification->show();
            });
        });

        // connect(&session_.nxi_session().command_system().command_input(), &nxi::command_input::event_reset, [this]() { command_menu_->hide(); });

        connect(&session_.nxi_session().buffer_system().group(ui_window()->id()), &nxi::buffer_group::event_action_update,
                [this](const nxi::suggestion_vector& suggestions) {
                    command_menu_->set_data(stz::make_observer(&suggestions));
                    command_menu_->exec();
                });

        connect(&session_.nxi_session(), &nxi::user::event_error, this, [](const QString& message) {
            auto* error = new QMessageBox;
            error->setAttribute(Qt::WA_DeleteOnClose, true);
            error->setWindowTitle("nxi error");
            error->setText(message);
            error->show();
        });

        setMouseTracking(true);
    }

    bool main::toggle_fullmode(int state)
    {
        auto toggle_fullscreen = [](auto this_, bool active) {
            if (active)
            {
                this_->window()->showFullScreen();
                this_->control_bar_->hide();
            }
            else
            {
                this_->window()->showNormal();
                this_->control_bar_->showNormal();
            }
        };
        auto toggle_fullwindow = [](auto this_, bool active) {
            if (active) this_->control_bar_->hide();
            else this_->control_bar_->showNormal();
        };

        auto fullscreen_mode = session_.nxi_session().config().browser.page_fullscreen_mode.get();

        switch (static_cast<nxi::config::fullscreen_mode>(fullscreen_mode))
        {
        case nxi::config::fullscreen_mode::window:
            main_interface::toggle_fullmode();
            toggle_fullwindow(this, fullmode());
            return true;

        case nxi::config::fullscreen_mode::screen:
            main_interface::toggle_fullmode();
            toggle_fullscreen(this, fullmode());
            return true;

        case nxi::config::fullscreen_mode::hybrid:
            main_interface::toggle_fullmode(3);
            if (fullmode() == 1) toggle_fullwindow(this, true);
            else if (fullmode() == 2) toggle_fullscreen(this, true);
            else toggle_fullscreen(this, false);
            return fullmode() != 2;
            break;
        }

        return fullmode();
    }

    void main::focusInEvent(QFocusEvent* event)
    {
        QWidget::focusInEvent(event);
        if (fullmode() != 0) control_bar_->show();
    }

    void main::focusOutEvent(QFocusEvent* event)
    {
        QWidget::focusOutEvent(event);
        if (fullmode() != 0) control_bar_->hide();
    }

    void main::paintEvent(QPaintEvent*)
    {
        auto& image = style_data.background_image;

        auto d_y = image.height() - height();

        QRectF source(0.0, 0.0, image.width(), image.height());
        QRectF target(0, -d_y, image.width(), image.height());

        QPainter painter(this);
        painter.fillRect(0, 0, width(), height(), style_data.background_color);
        // painter.drawImage(target, image, source);

        // insane design drawing
        // auto& design_color = session_.nxi_session().interface_system().style().data().field.background_color.get();
        // painter.fillRect(0, 0 , width(), control_bar_->height(), design_color);
    }

    void main::resizeEvent(QResizeEvent*)
    {
        command_menu_->setFixedWidth(control_bar_->width());
        command_menu_->move(control_bar_->x(), control_bar_->height());
    }
} // ui::interfaces