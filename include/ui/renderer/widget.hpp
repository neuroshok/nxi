#ifndef UI_RENDERER_WIDGET_H_NXI
#define UI_RENDERER_WIDGET_H_NXI

#include <ui/renderer.hpp>

class QWidget;
class QHBoxLayout;

namespace ui
{
    class widget_renderer : public ui::renderer, public QWidget
    {
    public:
        widget_renderer();

        void display(node_page*);
        void display(widget_page*);
        nxi::renderer_type type() const override;
        QWidget* widget() override { return this; }

    private:
        QHBoxLayout* layout_;
    };
} // ui

#endif // UI_RENDERER_WIDGET_H_NXI