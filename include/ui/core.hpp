#ifndef UI_CORE_H_NXI
#define UI_CORE_H_NXI

#include <ui/system/page.hpp>
#include <ui/system/user.hpp>

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
        ui::user_system& user_system();

    private:
        QApplication& app_;
        nxi::core& nxi_core_;

        ui::user_system user_system_;

        QSystemTrayIcon* systray_;
    };
} // ui

#endif // UI_CORE_H_NXI