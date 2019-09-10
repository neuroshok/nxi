#ifndef INCLUDE_NXW_MENU_ITEM_HPP_NXI
#define INCLUDE_NXW_MENU_ITEM_HPP_NXI

#include <functional>

#include <QFrame>

class QString;

namespace nxw
{
    class menu_item : public QFrame
    {
        Q_OBJECT
    public:
        enum class states { idle, hover, selected };

        menu_item(const QString& str_name, std::function<void()> command = {}, const QString& str_icon = "");

        void set_state(states state);

        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    private:
        std::function<void()> command_;
    };
} // nxw

#endif // INCLUDE_NXW_MENU_ITEM_HPP_NXI
