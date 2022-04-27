#include <ui/command/input.hpp>

#include <nxi/command/executor.hpp>
#include <nxi/command/input.hpp>
#include <nxi/core.hpp>

#include <ui/core.hpp>
#include <ui/menu.hpp>
#include <ui/window.hpp>
#include <ui/command/menu.hpp>

#include <QKeyEvent>
#include <QPaintEvent>
#include <QLabel>
#include <nxi/system/interface.hpp>
#include <nxi/style.hpp>

namespace ui
{
    command_input::command_input(ui::session& session, QWidget* parent)
        : QLineEdit(parent)
        , session_{ session }
    {
        setStyleSheet("font-weight:bold");

        set_mode(nxi::command_input::mode_type::display);

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
            if (command_executor_ && session_.nxi_session().context_system().is_focus<nxi::contexts::command_executor>())
            {
                //if (nxi_input().suggestions().has_selection()) command_executor_->add_suggestion(nxi_input().suggestions().selected());
                command_executor_->add_value(nxi_input().text());
                command_executor_->exec();
                if (command_executor_->is_complete())
                {
                    nxi_input().reset();
                    session_.nxi_session().context_system().del<nxi::contexts::command_executor>();
                }
                else
                {
                    set_executor_placeholder(command_executor_->active_parameter().name());
                    setText("");
                }
            }
            else nxi_input().exec();
        });

        connect(&session_.nxi_session().context_system(), &nxi::context_system::event_context_add,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command_executor& ex){ set_executor_placeholder(ex.data.active_parameter().name()); }
            , [this](auto&&) { setPlaceholderText(default_placeholder_text); }
            );
        });

        connect(&session_.nxi_session().context_system(), &nxi::context_system::event_focus_context_update,
        [this](const nxi::context& context)
        {
            context.apply(
            [this](const nxi::contexts::command_executor& ex){ set_executor_placeholder(ex.data.active_parameter().name()); }
            , [this](auto&&) { setPlaceholderText(default_placeholder_text); }
            );
        });

        connect(&nxi_input().suggestions(), &nxi::suggestion_vector::event_selection_update,
        [this](int index)
        {
            if (session_.nxi_session().context_system().is_focus<nxi::contexts::command_executor>())
            {
                if (nxi_input().suggestions().has_selection())
                {
                    auto suggestion_text = nxi_input().suggestions().selected().text();
                    auto input_text_size = text().size() - selectedText().size();
                    setText(suggestion_text);
                    setSelection(input_text_size, suggestion_text.size() - input_text_size);
                }
            }
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
                if (command_executor_ && session_.nxi_session().context_system().is_active<nxi::contexts::command_executor>())
                    session_.nxi_session().context_system().del<nxi::contexts::command_executor>();
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

            case Qt::Key_Backspace:
                setText(text().mid(0, text().size()));
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
        if (focused_page) setText(focused_page->name());
        first_focus_ = true;
        set_mode(nxi::command_input::mode_type::display);
    }

    void command_input::enterEvent(QEnterEvent* event)
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

    void command_input::mouseMoveEvent(QMouseEvent* event)
    {
        if (nxi_input().mode() == nxi::command_input::mode_type::display) event->ignore();
        else QLineEdit::mouseMoveEvent(event);
    }

    void command_input::mousePressEvent(QMouseEvent* event)
    {
        mouse_press_origin_ = event->globalPosition().toPoint();
        if (nxi_input().mode() == nxi::command_input::mode_type::display) event->ignore();
        else QLineEdit::mousePressEvent(event);
    }

    void command_input::mouseReleaseEvent(QMouseEvent* event)
    {
        if (nxi_input().mode() == nxi::command_input::mode_type::input) return QLineEdit::mouseReleaseEvent(event);

        if (mouse_press_origin_ == event->globalPosition().toPoint())
        {
            set_mode(nxi::command_input::mode_type::input);

            if (first_focus_)
            {
                if (nxi_input().is_empty())
                {
                    auto focused_page = session_.nxi_session().page_system().focus();
                    if (focused_page) setText(focused_page->command());
                }
                else setText(nxi_input().text());
                selectAll();
                first_focus_ = false;
            }
        }
    }


    void command_input::paintEvent(QPaintEvent* event)
    {


        QLineEdit::paintEvent(event);
    }

    nxi::command_input& command_input::nxi_input()
    {
        return session_.nxi_session().command_system().command_input();
    }

    void command_input::set_executor_placeholder(const QString& parameter_name)
    {
        setPlaceholderText("Enter parameter `" + parameter_name + "` for " + command_executor_->command().name() + " (ESC to cancel)");
    }

    void command_input::set_mode(nxi::command_input::mode_type mode)
    {
        nxi_input().set_mode(mode);
        if (mode == nxi::command_input::mode_type::display)
        {
            QWidget::setCursor(QCursor(Qt::ArrowCursor));
        }
        else
        {
            QWidget::setCursor(QCursor(Qt::IBeamCursor));
        }
    }

    void command_input::style_update(ui::command_input* ui)
    {
        QPalette palette;
        palette.setColor(QPalette::Highlight, ui->style_data.selection_highlight);
        palette.setColor(QPalette::HighlightedText, ui->style_data.selection_highlight_text);
        palette.setColor(QPalette::Text, ui->style_data.text_color);
        ui->setPalette(palette);
    }
} // ui