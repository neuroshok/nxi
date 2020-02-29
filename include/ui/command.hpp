#ifndef INCLUDE_UI_COMMAND_HPP_NXI
#define INCLUDE_UI_COMMAND_HPP_NXI

#include <QLineEdit>
#include <QString>

class QLabel;
class QResizeEvent;
class QKeyEvent;
class QEvent;

#include <QEvent>
#include <include/nxi/system/command.hpp>
#include <nxi/command/input.hpp>

namespace nxw { class menu; }
namespace nxi { class style; }

namespace ui
{
    class core;
    class command_menu;

    class command : public QLineEdit
    {
    public:
        command(ui::core& ui_core);

        void resizeEvent(QResizeEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;

        void focusInEvent(QFocusEvent* event) override;
        void focusOutEvent(QFocusEvent* event) override;

        void paintEvent(QPaintEvent*) override;

    private:
        nxi::command_input& command_input();

    private:
        ui::core& ui_core_;

        QLabel* info_;
        QLabel* header_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_HPP_NXI
