#ifndef INCLUDE_UI_UTILITY_HPP_NXI
#define INCLUDE_UI_UTILITY_HPP_NXI

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>

namespace ui
{
    inline QPixmap make_pixmap_from_svg(const QString& svg, const QSize& size, QColor foreground_color, QColor background_color = { 0, 0, 0 })
    {
        QSvgRenderer svg_renderer{ svg };
        QImage source{ size, QImage::Format_RGBA8888 };
        source.fill(0x00000000);
        QPainter painter;
        painter.begin(&source);
        svg_renderer.render(&painter, QRectF(0, 0, size.width(), size.height()));
        painter.end();

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

        return QPixmap::fromImage(image);
    }
} // ui

#endif // INCLUDE_UI_UTILITY_HPP_NXI