#include <ui/command/menu.hpp>

#include <nxi/core.hpp>
#include <nxi/command/input.hpp>
#include <nxi/command.hpp>

#include <ui/core.hpp>

#include <QPainter>

namespace ui
{
    command_menu::command_menu(ui::core& ui_core, QWidget* parent)
        : QWidget(parent)
        , ui_core_{ ui_core }
        , selection_index_{ -1 }
        , movie_{ ":/image/sound" }
    {
        connect(&movie_, &QMovie::frameChanged, [this](int){ repaint(); });
        movie_.start();

        connect(&ui_core_.nxi_core().command_system().command_input(), &nxi::command_input::event_selection_update, [this](int index)
        {
            selection_index_ = index;
            repaint();
        });
    }

    void command_menu::exec()
    {
        show();
    }

    void command_menu::set_data(stz::observer_ptr<nxi::commands_view> data)
    {
        commands_ = data;

        setContentsMargins(5, 5, 5, 5);
        setFixedHeight(static_cast<int>(commands_->size()) * (style_data.item_height + 1) + contentsMargins().top() + contentsMargins().bottom());

        repaint();
    }

    void command_menu::set_data(stz::observer_ptr<const nxi::suggestion_vector> suggestions)
    {
        suggestions_.swap(suggestions);

        setContentsMargins(5, 5, 5, 5);
        setFixedHeight(static_cast<int>(suggestions_->size()) * (style_data.item_height + 1) + contentsMargins().top() + contentsMargins().bottom());

        repaint();
    }

    void command_menu::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        painter.fillRect(rect(), style_data.background_color);

        int item_y = contentsMargins().top();
        int item_x = contentsMargins().left();
        int item_index = 0;

        for (const auto& suggestion : *suggestions_)
        {
            QRect item_rect{ item_x, item_y, width() - contentsMargins().left() - contentsMargins().right(), style_data.item_height };
            bool selected = false;
            if (item_index == selection_index_) selected = true;

            suggestion.visit(
              [this, &item_rect, &selected](nds::node_ptr<const nxi::command> s) { draw_item(s, item_rect, selected); }
            , [this, &item_rect, &selected](nds::node_ptr<const nxi::page> s) { draw_item(s, item_rect, selected); }
            , [this, &item_rect, &selected, &suggestion](auto&& s) { draw_item(suggestion, item_rect, selected); }
            );

            item_y += style_data.item_height + 1;
            item_index++;
        }
    }



    // draw command
    void command_menu::draw_item(nds::node_ptr<const nxi::command> node_command,  QRect& item_rect, bool selected)
    {
        const nxi::command& command = *node_command;
        QPainter painter(this);
        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.background_color);

        // command icon
        QRect icon_rect{ item_rect.left(), item_rect.top(), style_data.item_height, style_data.item_height };
        QPixmap icon{ command.icon() };
        QRect source_icon_rect = icon.rect();
        source_icon_rect.moveCenter(icon_rect.center());

        painter.drawPixmap(source_icon_rect, icon);
        item_rect.setLeft( item_rect.left() + icon_rect.width());

        // command name
        painter.setPen(style_data.item_text_color);
        painter.drawText(item_rect, Qt::AlignVCenter, command.name());

        auto& command_input = ui_core_.nxi_core().command_system().command_input().text();
        int hl_offset = command.name().indexOf(command_input);
        if (hl_offset >= 0)
        {
            QRect hl_rect{ item_rect };
            QString hl_text = command.name().mid(hl_offset, command_input.size());
            hl_rect.setLeft(item_rect.left() + painter.fontMetrics().size(Qt::TextSingleLine, command.name().mid(0, hl_offset)).width());
            painter.setPen(Qt::green);
            painter.drawText(hl_rect, Qt::AlignVCenter, hl_text);
        }

        item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, command.name()).width());

        // command desc
        if (!command.description().isEmpty())
        {
            QFont font;
            font.setItalic(true);
            painter.setFont(font);
            painter.setPen(style_data.item_text_color.darker(120));
            painter.drawText(item_rect, Qt::AlignVCenter, command.description());
            item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, command.description()).width());
        }

        // command shortcut
        if (!command.shortcut().is_empty())
        {
            QFont font;
            QString shortcut_text = command.shortcut().to_string();
            painter.setFont(font);
            painter.setPen(style_data.item_text_color.darker(80));
            painter.drawText(item_rect, Qt::AlignVCenter, shortcut_text);
            item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, shortcut_text).width());
        }

        // sound icon
        //QPixmap currentFrame = movie_.currentPixmap();
        //painter.drawPixmap(item_rect.left(), item_rect.top(), currentFrame);
    }

    void command_menu::draw_item(nds::node_ptr<const nxi::page> node_page, QRect& item_rect, bool selected)
    {
        const nxi::page& page = *node_page;
        QPainter painter(this);
        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.background_color);

        // page name
        painter.setPen(style_data.item_text_color);
        painter.drawText(item_rect, Qt::AlignVCenter, page.name());
        item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, page.name()).width());
    }

    void command_menu::draw_item(const nxi::suggestion& suggestion,  QRect& item_rect, bool selected)
    {
        QPainter painter(this);

        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.background_color);

        // suggestion icon
        QRect icon_rect{ item_rect.left(), item_rect.top(), style_data.item_height, style_data.item_height };
        QPixmap icon{ suggestion.icon() };
        QRect source_icon_rect = icon.rect();
        source_icon_rect.moveCenter(icon_rect.center());

        painter.drawPixmap(source_icon_rect, icon);
        item_rect.setLeft( item_rect.left() + icon_rect.width());

        // suggestion name
        painter.setPen(style_data.item_text_color);
        painter.drawText(item_rect, Qt::AlignVCenter, suggestion.text());
        item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, suggestion.text()).width());

        // suggestion info
        if (!suggestion.info().isEmpty())
        {
            QFont font;
            font.setItalic(true);
            painter.setFont(font);
            painter.setPen(style_data.item_text_color.darker(120));
            painter.drawText(item_rect, Qt::AlignVCenter, suggestion.info());
            item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, suggestion.info()).width());
        }
    }

    void command_menu::leaveEvent(QEvent* event)
    {
        hide();
    }

    void command_menu::wheelEvent(QWheelEvent* event)
    {
        if (event->delta() > 0)
            ui_core_.nxi_core().command_system().command_input().select_previous_suggestion();
        else
            ui_core_.nxi_core().command_system().command_input().select_next_suggestion();
    }
} // ui
