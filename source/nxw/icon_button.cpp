#include <nxw/icon_button.hpp>
#include "nxi/log.hpp"

#include <nxi/core.hpp>
#include <nxi/system/command.hpp>
#include <nxi/user.hpp>

#include <ui/system/user.hpp>
#include <ui/user.hpp>
#include <ui/utility.hpp>

namespace nxw
{
    icon_button::icon_button(ui::user& user, QWidget* parent, const QString& icon_path, QString str_command)
        : icon_button(user, parent, icon_path, std::move(str_command), style_type{})
    {}

    icon_button::icon_button(ui::user& user, QWidget* parent, const QString& icon_path, QString str_command, icon_button::style_type custom_style)
        : user_{ user }
        , str_command_{ std::move(str_command) }
        , style_data{ std::move(custom_style) }
    {
        // todo icon provider to cache conversions
        auto vs = user_.nxi_user().command_system().search(str_command_);
        if (!vs.empty()) command_ = vs[0];
        else nxi_warning("command {} not found", str_command_);

        if (command_) setToolTip(command_->description());
        else nxi_warning("command {} does not exist", str_command_);

        setFixedSize(style_data.size);
        QSize size{ style_data.size.width() - style_data.padding * 2, style_data.size.height() - style_data.padding * 2 };
        icon_ = ui::make_pixmap_from_svg(icon_path, size, style_data.icon_color, style_data.background_color);
        icon_hover_ = ui::make_pixmap_from_svg(icon_path, size, style_data.icon_color_hover, style_data.background_color_hover);

        connect(this, &QPushButton::clicked, [this] {
            if (command_) command_->exec();
        });
    }

    QImage icon_button::make_colorized_image(const QImage& source, QColor foreground_color, QColor background_color)
    {
        QImage image{ source.size(), source.format() };
        image.fill(0x00000000);

        for (int x = 0; x < source.width(); ++x)
        {
            for (int y = 0; y < source.height(); ++y)
            {
                auto source_color = source.pixelColor(x, y);
                if (source_color.alpha() > 0)
                {
                    foreground_color.setAlpha(source_color.alpha());
                    image.setPixelColor(x, y, foreground_color);
                }
                else
                {
                    background_color.setAlpha(source_color.alpha());
                    image.setPixelColor(x, y, background_color);
                }
            }
        }
        return image;
    }
    void icon_button::paintEvent(QPaintEvent*)
    {
        QPainter painter{ this };

        if (underMouse())
        {
            if (style_data.background_color_hover.alpha() > 0) painter.fillRect(0, 0, style_data.size.width(), style_data.size.height(), style_data.background_color_hover);
            painter.drawPixmap(style_data.padding, style_data.padding, icon_hover_);
        }
        else
        {
            if (style_data.background_color.alpha() > 0) painter.fillRect(0, 0, style_data.size.width(), style_data.size.height(), style_data.background_color);
            painter.drawPixmap(style_data.padding, style_data.padding, icon_);
        }
    }
} // nxw