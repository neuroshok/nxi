#ifndef INCLUDE_UI_INTERFACE_LIGHT_MAIN_HPP_NXI
#define INCLUDE_UI_INTERFACE_LIGHT_MAIN_HPP_NXI

#include <ui/interface/main.hpp>

namespace ui
{
    class command_menu;
    class session;
    class window;

    namespace interfaces::standard
    {
        class content;
    } // interfaces
} // ui

namespace ui::interfaces::light
{
    class control_bar;

    class main : public ui::main_interface
    {
        Q_OBJECT
    public:
        main(ui::session&, ui::window*);

        bool toggle_fullmode(int) override;

    protected:
        void focusInEvent(QFocusEvent*) override;
        void focusOutEvent(QFocusEvent*) override;
        void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent*) override;

    private:
        ui::session& session_;

        ui::interfaces::standard::content* content_;
        ui::interfaces::light::control_bar* control_bar_;
        ui::command_menu* command_menu_;
    };
} // ui::interfaces::light

#endif // INCLUDE_UI_INTERFACE_LIGHT_MAIN_HPP_NXI