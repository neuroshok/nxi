#ifndef NXW_ICON_BUTTON_H_NXI
#define NXW_ICON_BUTTON_H_NXI

#include <QPushButton>
#include <QPainter>
#include <QPalette>
#include <QImage>
#include <QSvgRenderer>

//! style
//! window

namespace ui
{
    class session;
} // ui

namespace nxw
{
    class icon_button : public QPushButton
    {
        Q_OBJECT
    public:
        struct style_type
        {
            QColor background_color = { QRgba64::fromRgba64(0) };
            QColor background_color_hover = { 0xFF, 0xFF, 0xFF, 0x22 };
            QColor icon_color = { 0x00BFFF };
            QColor icon_color_hover = { 0x00BFFF };

            QSize size = { 24, 24 };
            int padding = { 4 };
        } style;

        icon_button(ui::session&, QWidget* parent, const QString& icon_path, QString command = "nxi:help"); // , nxi::style* = nullptr

        void paintEvent(QPaintEvent*) override;

    private:
        QImage make_colorized_image(const QImage& source, QColor foreground_color, QColor background_color);

        ui::session& session_;
        QString command_;
        QImage image_;
        QImage image_hover_;
        QSvgRenderer* svg_renderer_;
    };
} // nxw

#endif // NXW_ICON_BUTTON_H_NXI