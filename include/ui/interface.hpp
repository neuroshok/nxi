#ifndef UI_INTERFACE_H_NXI
#define UI_INTERFACE_H_NXI

#include <QWidget>

namespace ui
{
    class window;

    class interface : public QWidget
    {
    public:
        interface(const QString& name = "default");
        interface(const QString& name, ui::window*);

        bool fullmode() const;
        virtual void toggle_fullmode();

        ui::window* window() const;

    private:
        bool fullmode_;
    };
} // ui

#endif