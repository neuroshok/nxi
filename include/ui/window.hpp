#ifndef UI_WINDOW_H_NXI
#define UI_WINDOW_H_NXI

#include <stz/observer_ptr.hpp>

#include <QWidget>
#include <platform/window.hpp>

class QHBoxLayout;

namespace ui
{
    class window_system;
    class main_interface;

    class window : public platform::window
    {
    Q_OBJECT
    public:
        window(ui::window_system&, unsigned int m_id);
        ~window();

        void mouseReleaseEvent(QMouseEvent*) override;
        void closeEvent(QCloseEvent*) override;
        void resizeEvent(QResizeEvent*) override;

        ui::main_interface* main_interface();
        void set_interface(ui::main_interface* interface);
        void set_grip(QWidget*);
        void set_fullscreen();

        unsigned int id() const;
        ui::window_system& window_system();

    private:
        ui::window_system& window_system_;
        unsigned int id_;
        ui::main_interface* interface_;
        QHBoxLayout* layout_;
    };

} // ui

#endif // UI_WINDOW_H_NXI