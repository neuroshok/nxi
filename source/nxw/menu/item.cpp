#include <nxw/menu/item.hpp>
#include <nxw/hbox_layout.hpp>

#include <QFrame>
#include <QLabel>
#include <QVariant>
#include <QStyle>
#include <QDebug>
#include <QPainter>
#include <nxi/style.hpp>
#include <nxw/menu.hpp>

namespace nxw
{
    menu_item::menu_item(nxw::menu* menu, const QString& str_name, std::function<void()> command, const QString& str_icon)
        : menu_{ menu }
        , command_{ std::move(command) }
        , text_{ str_name }
    {
        setFixedHeight(menu->style_data.item_height);
    }



    void menu_item::mouseReleaseEvent(QMouseEvent* event)
    {
        if (command_) command_();
        QWidget::mouseReleaseEvent(event);
    }

    void menu_item::enterEvent(QEnterEvent* event)
    {
        states_.setFlag(QStyle::State_MouseOver);
        update();
    }

    void menu_item::leaveEvent(QEvent* event)
    {
        states_.setFlag(QStyle::State_MouseOver, false);
        update();
    }

    const QString& menu_item::text() const
    {
        return text_;
    }

    void menu_item::paintEvent(QPaintEvent* event)
    {
        QColor background_color = menu_->style_data.background_color;
        QColor text_color = menu_->style_data.item_text_color;
        if (states_.testFlag(QStyle::State_MouseOver))
        {
            background_color = menu_->style_data.item_background_color_hover;
            text_color = menu_->style_data.item_text_color_hover;
        }

        QPainter painter(this);
        painter.fillRect(rect(), background_color);

        QFont font = painter.font();
        font.setPixelSize(height() / 2);
        painter.setFont(font);
        painter.setPen(text_color);
        painter.drawText(rect(), Qt::AlignVCenter, text_);
    }

    void menu_item::set_text(const QString& text)
    {
        text_ = text;
    }
} // nxw