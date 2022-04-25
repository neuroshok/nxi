#include <nxw/icon_button.hpp>
#include "nxi/log.hpp"

#include <nxi/core.hpp>
#include <nxi/system/command.hpp>

#include <ui/system/session.hpp>
#include <ui/utility.hpp>

namespace nxw
{
    icon_button::icon_button(ui::session& session, QWidget* parent, const QString& icon_path, QString str_command)
        : session_{ session }
        , str_command_{ std::move(str_command) }
        , svg_renderer_{ new QSvgRenderer{ icon_path } }
    {
        connect(&session.nxi_core(), &nxi::core::event_load, [icon_path, this] {
            auto vs = session_.nxi_session().command_system().search(str_command_);
            if (!vs.empty()) command_ = vs[0];
            else nxi_warning("command {} not found", str_command_);

            if (command_) setToolTip(command_->description());
            else nxi_warning("command {} does not exist", str_command_);

            setFixedSize(style.size);
            QSize size{ style.size.width() - style.padding * 2, style.size.height() - style.padding * 2 };
            image_ = ui::make_image_from_svg(icon_path, size, style.icon_color, style.background_color);
            image_hover_ = ui::make_image_from_svg(icon_path, size, style.icon_color_hover, style.background_color_hover);
        });

        connect(this, &QPushButton::clicked, [this] {
            if (command_) command_->exec();
        });

        if (!svg_renderer_->isValid()) nxi_error("invalid icon");
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
            if (style.background_color_hover.alpha() > 0) painter.fillRect(0, 0, style.size.width(), style.size.height(), style.background_color_hover);
            painter.drawImage(style.padding, style.padding, image_hover_);
        }
        else
        {
            if (style.background_color.alpha() > 0) painter.fillRect(0, 0, style.size.width(), style.size.height(), style.background_color);
            painter.drawImage(style.padding, style.padding, image_);
        }
    }
} // nxw