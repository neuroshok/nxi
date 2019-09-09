#ifndef INCLUDE_UI_COMMAND_HPP_NXI
#define INCLUDE_UI_COMMAND_HPP_NXI

#include <QLineEdit>
#include <QString>

class QLabel;
class QResizeEvent;
class QKeyEvent;
class QEvent;

namespace nxw { class menu; }

namespace ui
{
    class core;
    class menu;

    class command : public QLineEdit
    {
    public:
        command(ui::core& ui_core);

        void resizeEvent(QResizeEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;

        void focusOutEvent(QFocusEvent* event) override;

    private:
        ui::core& ui_core_;
        QString input_;

        nxw::menu* menu_;
        QLabel* info_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_HPP_NXI
