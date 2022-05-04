#include <ui/command/menu.hpp>

#include <nxi/command.hpp>
#include <nxi/command/input.hpp>
#include <nxi/core.hpp>
#include <ui/core.hpp>
#include <QPainter>
#include <QPainterPath>
#include "ui/utility.hpp"

namespace ui
{
    command_menu::command_menu(ui::user_session& session, QWidget* parent)
        : QWidget(parent)
        , session_{ session }
        , hover_index_{ -1 }
        , selection_index_{ -1 }
    {
        setContentsMargins(5, 5, 5, 5);
        connect(&session.nxi_core(), &nxi::core::event_load, [ this]
        {
            QSize size{ style_data.item_height / 2 - style_data.control_padding, style_data.item_height / 2 - style_data.control_padding };
            image_copy_ = ui::make_image_from_svg(":/icon/copy", size, style_data.item_text_color);
            image_sound_ = ui::make_image_from_svg(":/icon/sound", size, style_data.item_text_color.darker(200));
            image_sound_muted_ = ui::make_image_from_svg(":/icon/sound", size, QColor{ 180, 20, 20 });
            image_sound_premuted_ = ui::make_image_from_svg(":/icon/sound", size, style_data.item_text_color.lighter(180));
        });


        connect(&session_.nxi_session().command_system().command_input().suggestions(), &nxi::suggestion_vector::event_update, [this](stz::observer_ptr<const nxi::suggestion_vector> suggestions)
        {
            set_data(suggestions);
        });

        connect(&session_.nxi_session().command_system().command_input().suggestions(), &nxi::suggestion_vector::event_selection_update, [this](int index)
        {
            selection_index_ = index;
            repaint();
        });

        connect(&session_.nxi_session().page_system(), &nxi::page_system::event_update, [this](nds::node_ptr<nxi::page> page)
        {
            repaint();
            update();
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

        setFixedHeight(static_cast<int>(commands_->size()) * (style_data.item_height + 1) + contentsMargins().top() * 2 + style_data.header_height - 1 + contentsMargins().bottom());

        repaint();
    }

    void command_menu::set_data(stz::observer_ptr<const nxi::suggestion_vector> suggestions)
    {
        suggestions_.swap(suggestions);

        setFixedHeight(static_cast<int>(suggestions_->size()) * (style_data.item_height + 1) + contentsMargins().top() * 2 + style_data.header_height - 1 + contentsMargins().bottom());

        repaint();
    }

    void command_menu::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);
        // background image
        auto& image = style_data.background_image;

        auto d_x = image.width() - width();
        QRectF source(0.0, 0.0, image.width(), image.height());
        QRectF target(d_x, 0, image.width(), image.height());
        painter.drawImage(source, image, target);
        painter.fillRect(rect(), style_data.background_color);

        draw_header();

        int item_y = contentsMargins().top() * 2 + style_data.header_height;
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
        QRect header_rect{ contentsMargins().top(), contentsMargins().left(), width() - contentsMargins().left() - contentsMargins().right(), style_data.header_height };
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
                items.push_back("main");
                items.push_back("dev");
                items.push_back("nxi");
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
        font.setPixelSize(style_data.header_height - 2);
        painter.setFont(font);
        painter.setPen(style_data.item_text_color);

        int header_item_padding = 20;
        int i = 0;
        QColor color;
        for (const auto& item : items)
        {
            // item_rect.setWidth(painter.fontMetrics().size(Qt::TextSingleLine, item).width() + header_item_padding);
            item_rect.setWidth(style_data.header_item_width);

            if (header_item_index_ == i) color = style_data.item_background_color_hover;
            else color = style_data.item_background_color;

            painter.fillRect(item_rect, color);

            QRect text_rect = item_rect;
            text_rect.moveLeft(text_rect.left() + header_item_padding / 2);
            painter.drawText(text_rect, Qt::AlignVCenter, item);
            item_rect.moveLeft(item_rect.left() + item_rect.width() + 2);
            ++i;
        }
    }

    // draw command
    void command_menu::draw_item(nds::node_ptr<const nxi::command> node_command,  QRect& item_rect, bool selected)
    {
        const nxi::command& command = *node_command;
        QPainter painter(this);
        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.item_background_color);

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
        QFont font;
        painter.setFont(font);

        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else if (page->has_color()) painter.fillRect(item_rect, page->color());
        else painter.fillRect(item_rect, style_data.item_background_color);

        // page icon
        QRect icon_rect{ item_rect.left(), item_rect.top(), style_data.item_height, style_data.item_height };
        QPixmap icon{ page->icon().pixmap(icon_rect.width(), icon_rect.height()) };
        QRect source_icon_rect = icon.rect();
        source_icon_rect.moveCenter(icon_rect.center());
        painter.drawPixmap(source_icon_rect, icon);
        item_rect.setLeft( item_rect.left() + icon_rect.width() + 2);

        // page name
        QString page_id = "#";// + QString::number(page->id()) + " [" + session_.nxi_core().session_system().get(page->session_id()).name() + "]";
        QString page_name = page->name();
        painter.setPen(style_data.item_text_color.lighter());
        painter.drawText(item_rect, Qt::AlignVCenter, page_id);
        font.setBold(true);
        painter.setFont(font);
        item_rect.moveLeft(item_rect.left() + painter.fontMetrics().size(Qt::TextSingleLine, page_id).width() + 4);
        painter.setPen(style_data.item_text_color);
        painter.drawText(item_rect, Qt::AlignVCenter, page->name());

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
        page_command_area_ = item_rect;
        font.setBold(false);
        painter.setFont(font);
        painter.setPen(style_data.item_text_color.darker(180));
        painter.drawText(item_rect, Qt::AlignVCenter, page->command());
        item_rect.setLeft(item_rect.left() + 16 + painter.fontMetrics().size(Qt::TextSingleLine, page->command()).width());

        // right page controls
        QRect control_rect{ item_rect.right() - style_data.item_height - 16, item_rect.top(), style_data.item_height, style_data.item_height };

        // muted state
        draw_image(painter, image_copy_, control_rect.left(), control_rect.top(), image_copy_.size(), style_data.control_padding);
        control_rect.moveLeft(control_rect.left() - style_data.item_height);

        page_mute_area_ = control_rect;
        if (page->is_muted()) draw_image(painter, image_sound_muted_, control_rect.left(), control_rect.top(), image_sound_muted_.size(), style_data.control_padding);
        else if (page->is_audible()) draw_image(painter, image_sound_, control_rect.left(), control_rect.top(), image_sound_.size(), style_data.control_padding);
        else draw_image(painter, image_sound_premuted_, control_rect.left(), control_rect.top(), image_sound_.size(), style_data.control_padding);
    }
    void command_menu::draw_item(const nxi::suggestion& suggestion,  QRect& item_rect, bool selected)
    {
        QPainter painter(this);

        if (selected) painter.fillRect(item_rect, style_data.item_background_color_selected);
        else painter.fillRect(item_rect, style_data.item_background_color);

        // suggestion icon
        QImage suggestion_icon{ suggestion.icon() };

        QRect icon_rect{ item_rect.left(), item_rect.top(), style_data.item_height, style_data.item_height };
        draw_image(painter, suggestion_icon, item_rect.left(), item_rect.top(), QSize{ style_data.item_height, style_data.item_height }, style_data.item_height / 4);

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
        header_item_index_ = -1;
        auto mouse_pos = mapFromGlobal(event->globalPosition());
        mouse_pos.setY(mouse_pos.y());

        // header area
        if (mouse_pos.y() >= contentsMargins().top() && mouse_pos.y() < contentsMargins().top() + style_data.header_height
            && mouse_pos.x() > contentsMargins().left())
        {
            auto width = (contentsMargins().left() + (style_data.header_item_width + 2) * 3) / 3;
            header_item_index_ = mouse_pos.x() / width;
        }

        // item area
        for (int i = 0; i < suggestions_->size(); ++i)
        {
            auto selected_top = contentsMargins().top() * 2 + style_data.header_height + style_data.item_height * i + (i * 1);
            auto selected_bottom = selected_top + style_data.item_height;

            if (mouse_pos.y() >= selected_top && mouse_pos.y() < selected_bottom)
            {
                hover_index_ = i;
                break;
            }
        }

        if (hover_index_ != -1 && hover_index_ != selection_index_)
            session_.nxi_session().command_system().command_input().suggestions().select(hover_index_);
        else if (hover_index_ == -1) session_.nxi_session().command_system().command_input().suggestions().select(-1);
    }

    void command_menu::mousePressEvent(QMouseEvent* event)
    {
        auto mouse_pos = mapFromGlobal(event->globalPosition()).toPoint();
        if (mouse_pos.x() > page_mute_area_.left() && mouse_pos.x() < page_mute_area_.right())
        {
            suggestions().selected().apply(
            [](nds::node_ptr<nxi::page> page) { page->toggle_mute(); }
            , [](auto&&) {});
        }

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

    void command_menu::draw_image(QPainter& painter, const QImage& image, int x, int y, QSize size, int margin)
    {
        x += (style_data.item_height - size.width()) / 2;
        y += (style_data.item_height - size.height()) / 2;
        QRect source_image_rect{ x, y, size.width(), size.height() };
        painter.drawImage(source_image_rect, image);
    }
} // ui