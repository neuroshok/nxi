#ifndef UI_WINDOW_H_NXI
#define UI_WINDOW_H_NXI

#include <stz/observer_ptr.hpp>

#include <platform/window.hpp>
#include <QWidget>

class QHBoxLayout;

namespace nxi
{
    class window;
} // nxi

namespace ui
{
    class user;
    class main_interface;
    class window_system;

    class window : public platform::window
    {
        Q_OBJECT
    public:
        window(ui::user&, nxi::window&);
        ~window();

        void mouseReleaseEvent(QMouseEvent*) override;
        void closeEvent(QCloseEvent*) override;
        void resizeEvent(QResizeEvent*) override;

        ui::main_interface* main_interface();
        void set_interface(ui::main_interface* interface);
        void set_grip(QWidget*);
        void set_fullscreen();

        [[nodiscard]] int id() const;
        [[nodiscard]] nxi::window& nxi_window() const;
        ui::window_system& window_system();

    private:
        ui::user& user_;
        ui::window_system& window_system_;
        nxi::window& nxi_window_;
        ui::main_interface* interface_;
        QHBoxLayout* layout_;
    };

} // ui

#endif // UI_WINDOW_H_NXI