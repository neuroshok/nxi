#ifndef NXW_ICON_BUTTON_H_NXI
#define NXW_ICON_BUTTON_H_NXI

#include <nds/graph/node.hpp>

#include <QPushButton>
#include <QPainter>
#include <QPalette>
#include <QImage>
#include <QSvgRenderer>

//! style
//! window

namespace nxi
{
    class command;
} // nxi
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
            QColor background_color_hover = { QRgba64::fromRgba64(0) };
            QColor icon_color = { 0x00BFFF };
            QColor icon_color_hover = { 0x00BFFF };

            QSize size = { 24, 24 };
            int padding = { 4 };
        } style_data;

        icon_button(ui::session&, QWidget* parent, const QString& icon_path, QString str_command = "nxi:help");
        icon_button(ui::session&, QWidget* parent, const QString& icon_path, QString str_command, style_type);

        void paintEvent(QPaintEvent*) override;

    private:
        QImage make_colorized_image(const QImage& source, QColor foreground_color, QColor background_color);

        ui::session& session_;
        nds::node_ptr<nxi::command> command_;
        QString str_command_;
        QImage image_;
        QImage image_hover_;
        QSvgRenderer* svg_renderer_;
    };
} // nxw

#endif // NXW_ICON_BUTTON_H_NXI