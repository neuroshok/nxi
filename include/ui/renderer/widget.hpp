#ifndef UI_RENDERER_WIDGET_H_NXI
#define UI_RENDERER_WIDGET_H_NXI

#include <ui/renderer.hpp>

class QWidget;
class QHBoxLayout;

namespace ui
{
    class widget_renderer : public ui::renderer
    {
    public:
        widget_renderer();

        void display(widget_page* p);
        nxi::renderer_type type() const override;

    private:
        QHBoxLayout* layout_;
    };
} // ui

#endif // UI_RENDERER_WIDGET_H_NXI