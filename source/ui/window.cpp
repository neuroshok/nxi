#include <ui/window.hpp>

#include <ui/core.hpp>
#include <ui/interface/main.hpp>
#include <ui/system/window.hpp>

#include <nxw/hbox_layout.hpp>


namespace ui
{
    window::window(ui::window_system& window_system, unsigned int id)
        : window_system_{ window_system }
        , id_{ id }
        , interface_{ nullptr }
    {
        layout_ = new nxw::hbox_layout;
        setLayout(layout_);
    }

    window::~window(){ }


    void window::mouseReleaseEvent(QMouseEvent* event)
    {
        if (platform::window::isMaximized()) return;
        window_system().move(this, x(), y());
        window_system().resize(this, width(), height());
    }

    void window::closeEvent(QCloseEvent* event)
    {
        window_system().close(this);
        deleteLater();
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
    }


    unsigned int window::id() const
    {
        return id_;
    }

    ui::window_system& window::window_system()
    {
        return window_system_;
    }

    void window::set_grip(QWidget* widget)
    {
        platform::window::set_grip(widget);
    }

    void window::set_fullscreen()
    {
        platform::window::set_fullscreen();
    }
} // ui