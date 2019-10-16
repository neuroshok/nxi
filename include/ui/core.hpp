#ifndef UI_CORE_H_NXI
#define UI_CORE_H_NXI

#include <ui/system/window.hpp>
#include <ui/system/page.hpp>

#include <QWidget>

namespace nxi { class core; }

class QApplication;
class QSystemTrayIcon;

namespace ui
{
    class main_interface;

    class page_system;
    class window_system;
    class window;

    class core : public QObject
    {
        Q_OBJECT
    public:
        core(QApplication& app, nxi::core& nxi_core);
        ~core() = default;
        core(const core&) = delete;
        core& operator=(const core&) = delete;

        void load();
        void quit();

        nxi::core& nxi_core();
        ui::page_system& page_system();
        ui::window_system& window_system();

        ui::main_interface* make_main_interface(ui::window* window) { return main_interface_(window); }
        void set_main_interface(std::function<ui::main_interface*(ui::window*)> fn) { main_interface_ = std::move(fn); }

    private:
        QApplication& app_;
        nxi::core& nxi_core_;

        std::function<ui::main_interface*(ui::window*)> main_interface_;

        ui::page_system page_system_;
        ui::window_system window_system_;

        QSystemTrayIcon* systray_;
    };
} // ui

#endif // UI_CORE_H_NXI