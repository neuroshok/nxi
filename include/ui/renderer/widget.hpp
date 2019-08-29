#ifndef UI_RENDERER_WIDGET_H_NXI
#define UI_RENDERER_WIDGET_H_NXI

#include <ui/renderer.hpp>

class QWidget;

namespace ui
{
    class widget_renderer : public renderer
    {
    public:
        widget_renderer() = default;

        void display(widget_page* p);
        QWidget* widget();

    };
} // ui

#endif // UI_RENDERER_WIDGET_H_NXI