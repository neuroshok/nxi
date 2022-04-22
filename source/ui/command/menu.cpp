#include <ui/command/menu.hpp>

#include <nxi/command.hpp>
#include <nxi/command/input.hpp>
#include <nxi/core.hpp>
#include <ui/core.hpp>
#include <QPainter>
#include <QPainterPath>

namespace ui
{
    command_menu::command_menu(ui::session& session, QWidget* parent)
        : QWidget(parent)
        , session_{ session }
        , hover_index_{ -1 }
        , selection_index_{ -1 }
        , movie_{ ":/image/sound" }
    {
        connect(&movie_, &QMovie::frameChanged, [this](int){ repaint(); });
        movie_.start();

        connect(&session_.nxi_session().command_system().command_input().suggestions(), &nxi::suggestion_vector::event_update, [this](stz::observer_ptr<const nxi::suggestion_vector> suggestions)
        {
            set_data(suggestions);
        });

        connect(&session_.nxi_session().command_system().command_input().suggestions(), &nxi::suggestion_vector::event_selection_update, [this](int index)
        {
            selection_index_ = index;
            repaint();
        });

        setMouseTracking(true);
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
        setFixedHeight(static_cast<int>(suggestions_->size()) * (style_data.item_height + 1) + contentsMargins().top() + style_data.item_height + contentsMargins().bottom());

        repaint();
    }

    void command_menu::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);
        // background image
        painter.fillRect(rect(), style_data.background_color);
        auto& image = style_data.background_image;

        auto d_x = image.width() - width();
        QRectF source(0.0, 0.0, image.width(), image.height());
        QRectF target(d_x, 0, image.width(), image.height());
        painter.drawImage(source, image, target);

        draw_header();

        int item_y = contentsMargins().top() + style_data.item_height + contentsMargins().top();
        int item_x = contentsMargins().left();
        int item_index = 0;

        for (const auto& suggestion : *suggestions_)
        {
            QRect item_rect{ item_x, item_y, width() - contentsMargins().left() - contentsMargins().right(), style_data.item_height };
            bool selected = false;
            if (item_index == selection_index_) selected = true;

            suggestion.apply(
              [this, &item_rect, &selected](nds::node_ptr<const nxi::command> s) { draw_item(s, item_rect, selected); }
            , [this, &item_rect, &selected](nds::node_ptr<nxi::page> s) { draw_item(nds::node_ptr<const nxi::page>{ s }, item_rect, selected); }
            , [this, &item_rect, &selected, &suggestion](auto&& s) { draw_item(suggestion, item_rect, selected); }
            );

            item_y += style_data.item_height + 1;
            item_index++;
        }
    }

    // draw header
    void command_menu::draw_header()
    {
        QPainter painter(this);
        QRect header_rect{ contentsMargins().top(), contentsMargins().left(), width() - contentsMargins().left() - contentsMargins().right(), style_data.item_height };
        painter.fillRect(header_rect, style_data.background_color.darker());

        QRect item_rect = header_rect;
        std::vector<QString> items;
        session_.nxi_session().context_system().apply_on_focus
        (
            [this, &items](const nxi::contexts::command&)
            {
                auto res = session_.nxi_session().command_system().root_sources();
                if (res.size() > 0) items.push_back(res[0]->action_name());

                items.push_back(session_.nxi_session().command_system().root()->action_name());
            }
            , [this, &items](const nxi::contexts::page&)
            {
                items.push_back("page1");
                items.push_back("page2");
                items.push_back("page3");
                items.push_back("page4");
                items.push_back("page5");
            }
            , [this, &items](const nxi::contexts::command_executor& ctx)
            {
                items.push_back(ctx.data.command().action_name());
                items.push_back(ctx.data.active_parameter().name());
            }
            , [this](auto&&) {  }
        );

        // display session_.nxi_session().page_system().root()
        // header with items
        QFont font;
        font.setBold(true);
        font.setPixelSize(style_data.item_height - 4);
        painter.setFont(font);
        painter.setPen(style_data.item_text_color);

        int header_item_padding = 40;
        int header_item_arrow_width = 12;
        int i = items.size() % 2;
        auto color1 = style_data.background_color.lighter();
        auto color2 = style_data.background_color.darker(180);
        auto color = color1;
        for (const auto& item : items)
        {
            item_rect.setWidth(painter.fontMetrics().size(Qt::TextSingleLine, item).width() + header_item_padding);

            if (++i % 2) color = color2;
            else color = color1;

            auto rcolor = color1;
            if (color == color1) rcolor = color2;

            painter.fillRect(item_rect, color);

            QRect item_rect_arrow = item_rect;
            item_rect_arrow.moveLeft(item_rect.x() + item_rect.width() - header_item_arrow_width);
            item_rect_arrow.setWidth(header_item_arrow_width);
            painter.fillRect(item_rect_arrow, rcolor);

            QPainterPath path;
            path.moveTo(item_rect.topRight().x() - header_item_arrow_width + 1, item_rect.topRight().y());

            path.lineTo(QPoint{ item_rect.bottomRight().x(), item_rect.topRight().y() + (item_rect.height() / 2)});
            path.lineTo(item_rect.bottomRight().x() - header_item_arrow_width + 1, item_rect.bottomRight().y());
            path.lineTo(item_rect.topRight().x() - header_item_arrow_width + 1, item_rect.topRight().y());

            painter.fillPath(path, color);

            QRect text_rect = item_rect;
            text_rect.moveLeft(text_rect.left() - (header_item_arrow_width / 2) + header_item_padding / 2);
            painter.drawText(text_rect, Qt::AlignVCenter, item);
            item_rect.moveLeft(item_rect.left() + item_rect.width());
        }
    }

    // draw command
    void command_menu::draw_item(nds::node_ptr<const nxi::command> node_command,  QRect& item_rect, bool selected)
    {
        const nxi::command& command = *node_command;
        QPainter painter(this);
        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.item_background_color_hover);

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

        QString input_text = session_.nxi_session().command_system().command_input().text();
        int hl_offset = command.name().indexOf(input_text);
        if (input_text.size() > 0 && hl_offset >= 0)
        {
            QRect hl_rect{ item_rect };
            QString hl_text = command.name().mid(hl_offset, input_text.size());
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

    void command_menu::draw_item(nds::node_ptr<const nxi::page> page, QRect& item_rect, bool selected)
    {
        QPainter painter(this);
        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else if (page->has_color()) painter.fillRect(item_rect, page->color());
        else painter.fillRect(item_rect, style_data.item_background_color_hover);

        // page icon
        QRect icon_rect{ item_rect.left(), item_rect.top(), style_data.item_height, style_data.item_height };
        icon_rect = icon_rect.marginsRemoved(QMargins(2, 2, 2, 2));
        //icon_rect.moveLeft(10);
        QPixmap icon{ page->icon().pixmap(icon_rect.width(), icon_rect.height()) };
        painter.drawPixmap(icon_rect, icon);
        item_rect.setLeft( item_rect.left() + icon_rect.width() + 2);

        // page name
        QString page_name = "#" + QString::number(page->id()) + " " + page->name();
        painter.setPen(style_data.item_text_color);
        painter.drawText(item_rect, Qt::AlignVCenter, page_name);

        // highlight
        QString input_text = session_.nxi_session().command_system().command_input().text();
        int hl_offset = page_name.indexOf(input_text);
        if (input_text.size() > 0 && hl_offset >= 0)
        {
            QRect hl_rect{ item_rect };
            QString hl_text = page_name.mid(hl_offset, input_text.size());
            hl_rect.setLeft(item_rect.left() + painter.fontMetrics().size(Qt::TextSingleLine, page_name.mid(0, hl_offset)).width());
            painter.setPen(Qt::green);
            painter.drawText(hl_rect, Qt::AlignVCenter, hl_text);
        }
        item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, page_name).width());

        // page command
        QFont font;
        //font.setItalic(true);
        painter.setFont(font);
        painter.setPen(style_data.item_text_color.darker(180));
        painter.drawText(item_rect, Qt::AlignVCenter, page->command());
    }

    void command_menu::draw_item(const nxi::suggestion& suggestion,  QRect& item_rect, bool selected)
    {
        QPainter painter(this);

        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.item_background_color_hover);

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

    void command_menu::mouseMoveEvent(QMouseEvent* event)
    {
        hover_index_ = -1;
        auto mouse_pos = mapFromGlobal(event->globalPosition());
        mouse_pos.setY(mouse_pos.y());

        for (int i = 0; i < suggestions_->size(); ++i)
        {
            auto selected_top = contentsMargins().top() + style_data.item_height + style_data.item_height * i + (i * 1);
            auto selected_bottom = selected_top + style_data.item_height;

            if (mouse_pos.y() >= selected_top && mouse_pos.y() < selected_bottom)
            {
                hover_index_ = i;
                break;
            }
        }

        if (hover_index_ != -1 && hover_index_ != selection_index_)
            session_.nxi_session().command_system().command_input().suggestions().select(hover_index_);
    }

    void command_menu::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
            session_.nxi_session().command_system().command_input().exec();
        else if (event->button() == Qt::MiddleButton)
        {
            if (suggestions().has_selection())
            {
                suggestions().selected().apply(
                [this](nds::node_ptr<nxi::page> page) {
                    // todo suggestion_vector should use use event_close
                    session_.nxi_session().command_system().command_input().suggestions().erase(page);
                    session_.nxi_session().page_system().close(page);
                },
                [this](auto&&) {});
            }
        }
    }

    void command_menu::wheelEvent(QWheelEvent* event)
    {
        if (event->angleDelta().y() > 0)
            session_.nxi_session().command_system().command_input().suggestions().select_previous();
        else
            session_.nxi_session().command_system().command_input().suggestions().select_next();
    }

    nxi::suggestion_vector& command_menu::suggestions() { return session_.nxi_session().command_system().command_input().suggestions(); }
} // ui