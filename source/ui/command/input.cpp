#include <ui/command/input.hpp>

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
    command_input::command_input(ui::session& session)
        : session_{ session }
    {
        setStyleSheet("font-weight:bold");

        info_ = new QLabel(this);
        header_ = new QLabel(this);

        connect(&session_.nxi_session().command_system(), &nxi::command_system::event_execution_request, [this](nds::node_ptr<const nxi::command> command)
        {
            command_executor_.emplace( command );
            session_.nxi_session().context_system().add<nxi::contexts::command_executor>(command_executor_.value());
            setText("");
        });

        connect(this, &QLineEdit::returnPressed, [this]()
        {
            if (command_executor_ && session_.nxi_session().context_system().is_active<nxi::contexts::command_executor>())
            {
                command_executor_->add_value(nxi_input().text());
                command_executor_->exec();
                if (command_executor_->is_complete())
                {
                    nxi_input().reset();
                    session_.nxi_session().context_system().del<nxi::contexts::command_executor>();
                }
                else
                {
                    setPlaceholderText("Enter parameter `" + command_executor_->active_parameter().name() + "`");
                    setText("");
                }
            }
            else nxi_input().exec();
        });

        connect(&session_.nxi_session().context_system(), &nxi::context_system::event_context_add,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command_executor& ex){ setPlaceholderText("Enter parameter `" + ex.data.active_parameter().name() + "`"); }
            , [this](auto&&) { setPlaceholderText("Enter command or use shortcut"); }
            );
        });

        connect(&session_.nxi_session().context_system(), &nxi::context_system::event_focus_context_update,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command_executor& ex){ setPlaceholderText("Enter parameter `" + ex.data.active_parameter().name() + "`"); }
            , [this](auto&&) { setPlaceholderText("Enter command or use shortcut"); }
            );
        });


        // use only base
        connect(&session_.nxi_session().page_system(), &nxi::page_system::event_focus, this, [this](nxi::page_system::page_ptr page)
        {
            setText(page->name());

        });

        /*
        connect(&session_.nxi_session().page_system(), &nxi::page_system::event_update_command, this, [this](nxi::page_system::page_ptr page)
        {
            setText(page->name());
        });*/

        connect(&session_.nxi_session().command_system().command_input(), &nxi::command_input::event_shortcut_input_update, this, [this](const QString& shortcut_input)
        {
            setPlaceholderText(shortcut_input);
        });

        connect(&session_.nxi_session().command_system().command_input(), &nxi::command_input::event_input_update, this, [this](const QString& input)
        {
            setText(input);
        });
    }

    void command_input::resizeEvent(QResizeEvent* event)
    {
        QLineEdit::resizeEvent(event);
        //menu_->setFixedWidth(width());
    }

    void command_input::keyPressEvent(QKeyEvent* event)
    {
        QLineEdit::keyPressEvent(event);

        switch (event->key())
        {
            case Qt::Key_Escape:
                nxi_input().reset();
                break;
            case Qt::Key_Up:
                nxi_input().suggestions().select_previous();
                break;
            case Qt::Key_Down:
                if (!nxi_input().suggestions().has_selection())
                {
                    if (nxi_input().is_empty()) nxi_input().context_suggest();
                    else nxi_input().update(text(), event);

                    nxi_input().suggestions().select_next();
                }
                else nxi_input().suggestions().select_next();
                break;

            default:
                session_.nxi_session().command_system().command_input().update(text(), event);
        }
    }
    void command_input::keyReleaseEvent(QKeyEvent* event)
    {
        session_.nxi_session().command_system().command_input().update(text(), event);
    }

    void command_input::focusInEvent(QFocusEvent *event)
    {
        QLineEdit::focusInEvent(event);
    }

    void command_input::focusOutEvent(QFocusEvent* event)
    {
        QLineEdit::focusOutEvent(event);
        auto focused_page = session_.nxi_session().page_system().focus();
        //if (focused_page) setText(focused_page->name());
    }

    void command_input::enterEvent(QEvent* event)
    {
        if (hasFocus()) return;
        //if (!input_.is_empty()) return;
        auto focused_page = session_.nxi_session().page_system().focus();
        //if (focused_page) setText(focused_page->name());
    }

    void command_input::leaveEvent(QEvent* event)
    {
        if (hasFocus()) return;
        //if (!input_.is_empty()) return;
        auto focused_page = session_.nxi_session().page_system().focus();
        //if (focused_page) setText(focused_page->name());
    }

    void command_input::paintEvent(QPaintEvent* event)
    {


        QLineEdit::paintEvent(event);
    }

    nxi::command_input& command_input::nxi_input()
    {
        return session_.nxi_session().command_system().command_input();
    }
} // ui