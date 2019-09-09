#ifndef NXW_ICON_BUTTON_H_NXI
#define NXW_ICON_BUTTON_H_NXI

#include <nxi/command.hpp>

#include <QPushButton>
#include <QBitmap>
#include <QPixmap>

namespace nxw
{
    class icon_button : public QPushButton
    {
        Q_OBJECT

        Q_PROPERTY(QColor mask READ mask WRITE set_mask)
        Q_PROPERTY(QColor mask_hover READ mask_hover WRITE set_mask_hover)

    public:
        icon_button(QWidget* parent, const QString& icon_path)
            : path_{ icon_path }
            , mask_{ QColor::fromRgb(0, 0, 0) }
        {
            setIcon(QIcon(icon_path));
            setFixedSize(32, 32);
        }

        void enterEvent(QEvent * event)
        {
            replace_color(mask_, mask_hover_);
            QPushButton::enterEvent(event);
        }

        void leaveEvent(QEvent * event)
        {
            replace_color(mask_hover_, mask_);
            QPushButton::leaveEvent(event);
        }

        void replace_color(const QColor& source, const QColor& target)
        {
            auto pixmap = icon().pixmap(icon().availableSizes().last());
            auto mask = pixmap.createMaskFromColor(source, Qt::MaskOutColor);
            pixmap.fill(target);
            pixmap.setMask(mask);

            setIcon(QIcon(pixmap));
        }

        QColor mask() const { return mask_; }
        void set_mask(QColor color) {  mask_ = color; replace_color(QColor(0, 0, 0), mask_); }
        QColor mask_hover() const { return mask_hover_; }
        void set_mask_hover(QColor color) {  mask_hover_ = color; }

    private:
        QString path_;
        QColor mask_;
        QColor mask_hover_;
    };
} // nxw

#endif // NXW_ICON_BUTTON_H_NXI