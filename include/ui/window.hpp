#ifndef UI_WINDOW_H_NXI
#define UI_WINDOW_H_NXI

#include <nxi/window.hpp>
#include <stz/observer_ptr.hpp>

#include <QWidget>
#include <platform/native_window.hpp>

class QHBoxLayout;

namespace ui
{
    class core;
    class window_system;
    class main_interface;

class window : public platform::native_window
    {
    Q_OBJECT
    public:
        window(ui::core& ui_core, unsigned int m_id);
        ~window();

        void mouseReleaseEvent(QMouseEvent* event) override;
        void closeEvent(QCloseEvent* event) override;

        ui::main_interface* main_interface();
        void set_interface(ui::main_interface* interface);
        void set_grip(QWidget*);
        void set_fullscreen();

        unsigned int id() const;
        ui::window_system& window_system();

    private:
        ui::core& ui_core_;
        unsigned int id_;
        QHBoxLayout* layout_;
        ui::main_interface* interface_;
    };

} // ui

#endif // UI_WINDOW_H_NXI