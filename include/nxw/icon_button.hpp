#ifndef NXW_ICON_BUTTON_H_NXI
#define NXW_ICON_BUTTON_H_NXI

#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QImage>
#include <QBitmap>

//! style
//! window

namespace nxw
{
    class icon_button : public QPushButton
    {
        Q_OBJECT
    public:
        struct style_type
        {
            QColor background_color = {0, 0, 0};
            QColor icon_color = {255, 0, 0};
            QColor icon_color_hover = {0, 255, 0};

            QSize size = {16, 16};
        } style;

        icon_button(QWidget* parent, const QString& icon_path) // , nxi::style* = nullptr
            : image_{ icon_path }
        {
            // style::update(this);
            setFixedSize(style.size);
        }

        void paintEvent(QPaintEvent*)
        {
            // if nxi_style return (nxi_style->paint(this)

            QPainter p(this);
            QColor color = style.icon_color;
            if (underMouse()) color = style.icon_color_hover;

            for (int x = 0; x < image_.width(); ++x)
            {
                for (int y = 0; y < image_.height(); ++y)
                {
                    if (image_.pixelColor(x, y).alpha() > 0)
                    {
                        if (image_.pixelColor(x, y).alpha() < 255) color = Qt::red;
                        p.setPen(color);
                        p.drawPoint(x, y);
                    }
                }
            }
        }

    private:
        QImage image_;
    };
} // nxw

#endif // NXW_ICON_BUTTON_H_NXI