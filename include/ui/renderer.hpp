#ifndef UI_RENDERER_H_NXI
#define UI_RENDERER_H_NXI

#include <nxi/type.hpp>
#include <stz/observer_ptr.hpp>

#include <QWidget>

namespace ui
{
    class page;
    class web_page;
    class widget_page;

    class renderer : public QWidget
    {
        Q_OBJECT
    public:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
        virtual ~renderer();

        virtual nxi::renderer_type type() const = 0;

        void display(web_page*);
        void display(widget_page*);

        static ui::renderer* make(stz::observer_ptr<page>);

    protected:
        renderer() = default;
    };
} // ui

#endif // UI_RENDERER_H_NXI