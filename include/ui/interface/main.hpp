#ifndef UI_INTERFACE_MAIN_H_NXI
#define UI_INTERFACE_MAIN_H_NXI

#include <ui/interface.hpp>

namespace ui
{
    class core;
    class command;
    class command_menu;
} // ui

namespace ui::interfaces
{
    class content;
    class control_bar;
    class page_bar;

    class main : public ui::interface
    {
        Q_OBJECT
    public:
        main(ui::core&, ui::window*);

        void toggle_fullmode() override;

    protected:
        void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent*) override;

    private:
        ui::core& ui_core_;

        ui::command* command_bar_;
        ui::command_menu* command_menu_;

        ui::interfaces::content* content_;
        ui::interfaces::page_bar* page_bar_;
        ui::interfaces::control_bar* control_bar_;
    };
} // ui::interfaces

#endif // UI_INTERFACE_MAIN_H_NXI