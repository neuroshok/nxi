#include <ui/command.hpp>

#include <nxi/core.hpp>

#include <ui/core.hpp>
#include <ui/menu.hpp>

#include <QKeyEvent>
#include <QLabel>

namespace ui
{
    command::command(ui::core& ui_core)
        : ui_core_{ ui_core }
    {
        info_ = new QLabel(this);
        menu_ = new ui::menu(this);
        menu_->setObjectName("ui_command_menu");
        menu_->show_at(this);

        menu_->add_bottom(info_);
        menu_->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
        menu_->setAttribute(Qt::WA_ShowWithoutActivating);
        menu_->hide();

        connect(this, &QLineEdit::editingFinished, [this]()
        {
            menu_->hide();
            menu_->clear();
        });

        connect(this, &QLineEdit::textChanged, [this]()
        {
            if (!hasFocus()) return;

            input_ = text();
            menu_->clear();
            auto result = ui_core_.nxi_core().command_system().search(input_);
            if (result.size() > 0)
            {
                for (auto cmd : result)
                {
                    menu_->add(*cmd);
                }

            }
            info_->setText("results : " + QString::number(result.size()));
            menu_->exec();

            if (input_.isEmpty())
            {
                auto focused_page = ui_core_.nxi_core().page_system().focus();
                blockSignals(true);
                if (focused_page.has_value()) setText(focused_page.value()->command());
                selectAll();
                blockSignals(false);
            }
        });

        connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&>(&nxi::page_system::event_focus), this, [this](nxi::page& page)
        {
            setText(page.name());
        });

    }

    void command::resizeEvent(QResizeEvent* event)
    {
        QLineEdit::resizeEvent(event);
        menu_->setFixedWidth(width());
    }

    void command::keyPressEvent(QKeyEvent* event)
    {
        QLineEdit::keyPressEvent(event);

        if (event->key() == Qt::Key_Down)
        {
            qDebug() << "down";
            menu_->focus_next();
        }
    }

    void command::focusOutEvent(QFocusEvent* event)
    {
        QLineEdit::focusOutEvent(event);
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->name());
    }

    void command::enterEvent(QEvent* event)
    {
        if (hasFocus()) return;
        if (!input_.isEmpty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->command());
    }

    void command::leaveEvent(QEvent* event)
    {
        if (hasFocus()) return;
        if (!input_.isEmpty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->name());
    }
} // ui