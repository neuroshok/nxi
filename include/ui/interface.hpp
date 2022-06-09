#ifndef UI_INTERFACE_H_NXI
#define UI_INTERFACE_H_NXI

#include <QWidget>

namespace ui
{
    class window;

    class interface : public QWidget
    {
    public:
        interface(const QString& name, ui::window*);
        interface(ui::window*);

        int group_id() const;
        ui::window* ui_window() const;
    };
} // ui

#endif