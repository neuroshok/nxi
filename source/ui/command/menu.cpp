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

        connect(&ui_core_.nxi_core().command_system().user_input(), &nxi::command_input::event_selection_update, [this](int index)
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
        setFixedHeight(commands_->size() * (style_data.item_height + 1) + contentsMargins().top() + contentsMargins().bottom());

        repaint();
    }

    void command_menu::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        painter.fillRect(rect(), style_data.background_color);

        int item_y = contentsMargins().top();
        int item_x = contentsMargins().left();
        int item_index = 0;

        if (commands_)
        {
            for (auto item : *commands_)
            {
                QRect item_rect{ item_x, item_y, width() - contentsMargins().left() - contentsMargins().right(), style_data.item_height };
                bool selected = false;
                if (item_index == selection_index_) selected = true;

                draw_item(item, item_rect, selected);
                item_y += style_data.item_height + 1;
                item_index++;
            }
        }
    }

    // draw command
    void command_menu::draw_item(stz::observer_ptr<nxi::command> command,  QRect& item_rect, bool selected)
    {
        QPainter painter(this);
        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.background_color);

        // icon
        QRect icon_rect{ item_rect.left(), item_rect.top(), style_data.item_height, style_data.item_height };
        QPixmap icon{ command->icon() };
        QRect source_icon_rect = icon.rect();
        source_icon_rect.moveCenter(icon_rect.center());

        painter.drawPixmap(source_icon_rect, icon);
        item_rect.setLeft( item_rect.left() + icon_rect.width());

        // command name
        painter.setPen(style_data.item_text_color);
        painter.drawText(item_rect, Qt::AlignVCenter, command->name());

        auto& user_input = ui_core_.nxi_core().command_system().user_input().text();
        int hl_offset = command->name().indexOf(user_input);
        if (hl_offset >= 0)
        {
            QRect hl_rect{ item_rect };
            QString hl_text = command->name().mid(hl_offset, user_input.size());
            hl_rect.setLeft(item_rect.left() + painter.fontMetrics().size(Qt::TextSingleLine, command->name().mid(0, hl_offset)).width());
            painter.setPen(Qt::green);
            painter.drawText(hl_rect, Qt::AlignVCenter, hl_text);
        }

        item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, command->name()).width());

        // command desc
        if (!command->description().isEmpty())
        {
            QFont font;
            font.setItalic(true);
            painter.setFont(font);
            painter.setPen(style_data.item_text_color.darker(120));
            painter.drawText(item_rect, Qt::AlignVCenter, command->description());
            item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, command->description()).width());
        }

        // sound icon
        QPixmap currentFrame = movie_.currentPixmap();
        painter.drawPixmap(item_rect.left(), item_rect.top(), currentFrame);
    }
} // ui
