#include <ui/command.hpp>

#include <nxi/command/executor.hpp>
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
        setStyleSheet("font-weight:bold");

        info_ = new QLabel(this);
        header_ = new QLabel(this);

        connect(&ui_core_.nxi_core().command_system(), &nxi::command_system::event_execution_request, [this](nds::node_ptr<const nxi::command> command)
        {
            command_executor_.emplace( command );
            ui_core_.nxi_core().context_system().add<nxi::contexts::command_executor>(command_executor_.value());
            setText("");
        });

        connect(this, &QLineEdit::returnPressed, [this]()
        {
            if (command_executor_ && ui_core_.nxi_core().context_system().is_active<nxi::contexts::command_executor>())
            {
                command_executor_->add_value(command_input().text());
                command_executor_->exec();
                if (command_executor_->is_complete())
                {
                    command_input().reset();
                    ui_core_.nxi_core().context_system().del<nxi::contexts::command_executor>();
                }
                else
                {
                    setPlaceholderText("Enter parameter `" + command_executor_->active_parameter().name() + "`");
                    setText("");
                }
            }
            else command_input().exec();
        });

        connect(&ui_core_.nxi_core().context_system(), &nxi::context_system::event_context_add,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command_executor& ex){ setPlaceholderText("Enter parameter `" + ex.data.active_parameter().name() + "`"); }
            , [this](auto&&) { setPlaceholderText("Enter command or use shortcut"); }
            );
        });

        connect(&ui_core_.nxi_core().context_system(), &nxi::context_system::event_focus_context_update,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command_executor& ex){ setPlaceholderText("Enter parameter `" + ex.data.active_parameter().name() + "`"); }
            , [this](auto&&) { setPlaceholderText("Enter command or use shortcut"); }
            );
        });


        // use only base
        connect(&ui_core_.nxi_core().page_system(), &nxi::page_system::event_focus, this, [this](nxi::page_system::page_ptr page)
        {
            setText(page->name());
        });

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_shortcut_input_update, this, [this](const QString& shortcut_input)
        {
            setPlaceholderText(shortcut_input);
        });

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_input_update, this, [this](const QString& input)
        {
            setText(input);
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
                command_input().suggestions().select_previous();
                break;
            case Qt::Key_Down:
                if (!command_input().suggestions().has_selection())
                {
                    if (command_input().is_empty()) command_input().context_suggest();
                    else command_input().update(text(), event);

                    command_input().suggestions().select_next();
                }
                else command_input().suggestions().select_next();
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
        if (focused_page) setText(focused_page->name());
    }

    void command::enterEvent(QEvent* event)
    {
        if (hasFocus()) return;
        //if (!input_.is_empty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page) setText(focused_page->name());
    }

    void command::leaveEvent(QEvent* event)
    {
        if (hasFocus()) return;
        //if (!input_.is_empty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page) setText(focused_page->name());
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