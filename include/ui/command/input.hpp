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
        Q_OBJECT
    public:
        struct style_type
        {
            QColor background_color{ 200, 200, 200 };
            QColor background_color_focus{ 0, 0, 0 };
            QColor selection_highlight{ 0, 0, 0 };
            QColor selection_highlight_text{ 150, 150, 150 };
            QColor text_color{ 100, 100, 200 };
            QColor text_color_focus{ 100, 100, 200 };
        } style_data;
        static void style_update(ui::command_input*);

        command_input(ui::session& ui_core, QWidget* parent);

        void resizeEvent(QResizeEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

        void mouseMoveEvent(QMouseEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;

        void focusInEvent(QFocusEvent* event) override;
        void focusOutEvent(QFocusEvent* event) override;

        void paintEvent(QPaintEvent*) override;

    private:
        static inline auto default_placeholder_text = "Enter a command, a resource location or search";

        void set_executor_placeholder(const QString& parameter_name);
        void set_mode(nxi::command_input::mode_type);

        nxi::command_input& nxi_input();

    private:
        ui::session& session_;

        bool first_focus_ = true;
        QPoint mouse_press_origin_;
        QLabel* info_;
        QLabel* header_;
        std::optional<nxi::command_executor> command_executor_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_INPUT_HPP_NXI