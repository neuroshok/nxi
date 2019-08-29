#ifndef UI_RENDERER_H_NXI
#define UI_RENDERER_H_NXI

#include <QWidget>
#include <include/stz/observer_ptr.hpp>

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

        virtual QWidget* widget() = 0;

        void display(web_page*);
        void display(widget_page*);

        static renderer* make(stz::observer_ptr<page>);

    protected:
        renderer() = default;
    };
} // ui

#endif // UI_RENDERER_H_NXI