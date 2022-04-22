#ifndef INCLUDE_NXW_MENU_ITEM_HPP_NXI
#define INCLUDE_NXW_MENU_ITEM_HPP_NXI

#include <functional>

#include <QFrame>
#include <QString>
#include <QtWidgets/QStyle>

class QLabel;

namespace nxw
{
    class menu;

    class menu_item : public QFrame
    {
        Q_OBJECT
    public:

        enum states { idle = 0, hover = 1, selected = 2};


        menu_item(nxw::menu*, const QString& str_name, std::function<void()> command = {}, const QString& str_icon = "");

        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

        void paintEvent(QPaintEvent*) override;

        void set_text(const QString&);
        const QString& text() const;

    private:
        nxw::menu* menu_;
        QStyle::State states_;
        QString text_;
        std::function<void()> command_;
    };
} // nxw

#endif // INCLUDE_NXW_MENU_ITEM_HPP_NXI