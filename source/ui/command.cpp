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
            user_input().exec();
            setText("");
        });

        connect(this, &QLineEdit::editingFinished, [this]()
        {
            user_input().reset();
        });

        // text changed by the user
        connect(this, &QLineEdit::textEdited, [this](const QString& text)
        {
            if (!hasFocus()) return;

            user_input().update(text);
        });

        connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&>(&nxi::page_system::event_focus), this, [this](nxi::page& page)
        {
            setText(page.name());
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
                user_input().reset();
                break;
            case Qt::Key_Up:
                user_input().select_previous_suggestion();
                break;
            case Qt::Key_Down:
                if (!user_input().has_selected_suggestion())
                {
                    if (user_input().is_empty()) user_input().suggest_command();
                    else user_input().update(text());

                    user_input().select_next_suggestion();
                }
                else user_input().select_next_suggestion();
                break;
        }
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

    nxi::command_input& command::user_input()
    {
        return ui_core_.nxi_core().command_system().user_input();
    }
} // ui