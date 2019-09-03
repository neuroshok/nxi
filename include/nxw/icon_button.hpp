#ifndef NXW_ICON_BUTTON_H_NXI
#define NXW_ICON_BUTTON_H_NXI

#include <nxi/command.hpp>

#include <QPushButton>

namespace nxw
{
    class icon_button : public QPushButton
    {
        Q_OBJECT

        Q_PROPERTY(QColor mask READ mask WRITE set_mask)

    public:
        icon_button(QWidget* parent, const QString& icon_path)
            : path_{ icon_path }
            , mask_{ QColor::fromRgb(0, 0, 0) }
        {
            setIcon(QIcon(icon_path));
            setFixedSize(32, 32);
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
        void set_mask(QColor color) {  replace_color(mask_, color); mask_ = color; }

    private:
        QString path_;
        QColor mask_;
    };
} // nxw

#endif // NXW_ICON_BUTTON_H_NXI