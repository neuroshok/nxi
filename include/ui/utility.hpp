#ifndef INCLUDE_UI_UTILITY_HPP_NXI
#define INCLUDE_UI_UTILITY_HPP_NXI

#include <QImage>
#include <QPainter>
#include <QSvgRenderer>

namespace ui
{
    QImage make_image_from_svg(const QString& svg, const QSize& size)
    {
        QSvgRenderer svg_renderer;
        QImage image{ size, QImage::Format_RGBA8888 };
        image.fill(0x00000000);
        QPainter painter;
        painter.begin(&image);
        svg_renderer.render(&painter, QRectF(0, 0, size.width(), size.height()));
        painter.end();
        return image;
    }
} // ui

#endif // INCLUDE_UI_UTILITY_HPP_NXI