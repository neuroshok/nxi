#ifndef INCLUDE_UI_INTERFACE_STANDARD_MAIN_HPP_NXI
#define INCLUDE_UI_INTERFACE_STANDARD_MAIN_HPP_NXI

#include <ui/interface/main.hpp>

namespace ui
{
    class session;
    class window;
} // ui

namespace ui::interfaces::standard
{
    class content;
    class control_bar;
    class page_bar;

    class main : public ui::main_interface
    {
        Q_OBJECT
    public:
        main(ui::session&, ui::window*);

        bool toggle_fullmode(int) override;

    protected:
        void paintEvent(QPaintEvent*) override;
        void resizeEvent(QResizeEvent*) override;

    private:
        ui::session& session_;

        content* content_;
        control_bar* control_bar_;
        page_bar* page_bar_;
    };
} // ui::interfaces::standard

#endif // INCLUDE_UI_INTERFACE_STANDARD_MAIN_HPP_NXI