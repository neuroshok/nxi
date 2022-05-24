#ifndef UI_RENDERER_H_NXI
#define UI_RENDERER_H_NXI

#include <nxi/type.hpp>
#include <stz/observer_ptr.hpp>

#include <QWidget>

namespace ui
{
    class page;

    class node_page;
    class web_page;
    class widget_page;

    class renderer
    {
    public:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
        virtual ~renderer();

        virtual QWidget* widget() = 0;
        [[nodiscard]] virtual nxi::renderer_type type() const = 0;

        void display(node_page*);
        void display(web_page*);
        void display(widget_page*);

        static ui::renderer* make(stz::observer_ptr<ui::page>);

    protected:
        renderer() = default;
    };
} // ui

#endif // UI_RENDERER_H_NXI