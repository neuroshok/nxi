#include <ui/command.hpp>

#include <nxi/core.hpp>

#include <ui/core.hpp>
#include <ui/menu.hpp>
#include <ui/command/menu.hpp>

#include <QKeyEvent>
#include <QPaintEvent>
#include <QLabel>
#include <nxi/system/interface.hpp>
#include <nxi/style.hpp>

namespace ui
{
    command::command(ui::core& ui_core)
        : ui_core_{ ui_core }
    {
        setPlaceholderText("Enter command or use shortcut");

        info_ = new QLabel(this);
        header_ = new QLabel(this);

        connect(this, &QLineEdit::returnPressed, [this]()
        {
            command_input().exec();
            setText("");
        });

        connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&>(&nxi::page_system::event_focus), this, [this](nxi::page& page)
        {
            setText(page.name());
        });

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_shortcut_input_update, this, [this](const QString& shortcut_input)
        {
            setPlaceholderText(shortcut_input);
        });

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_command_param_required, this, [this](const QString& param_name)
        {
            setPlaceholderText("Enter parameter '" + param_name + "' : ");
        });

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_reset, this, [this]()
        {
            setPlaceholderText("Enter command or use shortcut");
        });
    }

    void command::resizeEvent(QResizeEvent* event)
    {
        QLineEdit::resizeEvent(event);
        //menu_->setFixedWidth(width());
    }

    void command::keyPressEvent(QKeyEvent* event)
    {
        QLineEdit::keyPressEvent(event);

        switch (event->key())
        {
            case Qt::Key_Escape:
                command_input().reset();
                break;
            case Qt::Key_Up:
                command_input().select_previous_suggestion();
                break;
            case Qt::Key_Down:
                if (!command_input().has_selected_suggestion())
                {
                    if (command_input().is_empty()) command_input().suggest_command();
                    else command_input().update(text(), event);

                    command_input().select_next_suggestion();
                }
                else command_input().select_next_suggestion();
                break;

            default:
                ui_core_.nxi_core().command_system().command_input().update(text(), event);
        }
    }
    void command::keyReleaseEvent(QKeyEvent* event)
    {
        ui_core_.nxi_core().command_system().command_input().update(text(), event);
    }

    void command::focusInEvent(QFocusEvent *event)
    {
        QLineEdit::focusInEvent(event);
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
        //if (!input_.is_empty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->command());
    }

    void command::leaveEvent(QEvent* event)
    {
        if (hasFocus()) return;
        //if (!input_.is_empty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->name());
    }

    void command::paintEvent(QPaintEvent* event)
    {


        QLineEdit::paintEvent(event);
    }

    nxi::command_input& command::command_input()
    {
        return ui_core_.nxi_core().command_system().command_input();
    }
} // ui