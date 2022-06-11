#include <ui/window.hpp>

#include <nxi/system/interface.hpp>
#include <nxi/user.hpp>
#include <nxi/window.hpp>

#include <ui/core.hpp>
#include <ui/interface/main.hpp>
#include <ui/system/window.hpp>
#include <ui/user.hpp>

#include <nxw/hbox_layout.hpp>

namespace ui
{
    window::window(ui::user& user, nxi::window& window)
        : user_{ user }
        , window_system_{ user_.window_system() }
        , nxi_window_{ window }
        , interface_{ nullptr }
    {
        layout_ = new nxw::hbox_layout;
        setLayout(layout_);
    }

    window::~window() = default;

    void window::mouseReleaseEvent(QMouseEvent* event)
    {
        if (platform::window::isMaximized()) return;
        window_system().move(this, x(), y());
        window_system().resize(this, width(), height());
    }

    void window::closeEvent(QCloseEvent* event)
    {
        // window_system().close(id());
        //  todo user_.nxi_user().window_system().close(id);
    }

    void window::resizeEvent(QResizeEvent*)
    {
        window_system().resize(this, width(), height());
    }

    ui::main_interface* ui::window::main_interface()
    {
        nxi_assert(interface_ != nullptr);
        return interface_;
    }

    void window::set_interface(ui::main_interface* interface)
    {
        interface_ = interface;
        layout_->addWidget(interface_);
        user_.nxi_user().interface_system().style().update(interface);
    }

    int window::id() const { return nxi_window_.id(); }

    ui::window_system& window::window_system() { return window_system_; }

    void window::set_grip(QWidget* widget) { platform::window::set_grip(widget); }

    void window::set_fullscreen() { platform::window::set_fullscreen(); }
    nxi::window& window::nxi_window() const { return nxi_window_; }
} // ui