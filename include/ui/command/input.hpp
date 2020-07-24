#ifndef INCLUDE_UI_COMMAND_INPUT_HPP_NXI
#define INCLUDE_UI_COMMAND_INPUT_HPP_NXI

#include <QLineEdit>
#include <QString>

class QLabel;
class QResizeEvent;
class QKeyEvent;
class QEvent;

#include <QEvent>
#include <nxi/system/command.hpp>
#include <nxi/command/input.hpp>
#include <nxi/command/executor.hpp>
#include <optional>

namespace nxw { class menu; }
namespace nxi { class style; }

namespace ui
{
    class command_menu;
    class session;

    class command_input : public QLineEdit
    {
    public:
        command_input(ui::session& ui_core);

        void resizeEvent(QResizeEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;

        void focusInEvent(QFocusEvent* event) override;
        void focusOutEvent(QFocusEvent* event) override;

        void paintEvent(QPaintEvent*) override;

    private:
        nxi::command_input& nxi_input();

    private:
        ui::session& session_;

        QLabel* info_;
        QLabel* header_;
        std::optional<nxi::command_executor> command_executor_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_INPUT_HPP_NXI
