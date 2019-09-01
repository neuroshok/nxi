#include <ui/renderer.hpp>

#include <ui/page.hpp>
#include <ui/renderer/web.hpp>
#include <ui/renderer/widget.hpp>

#include <QDebug>
#include <include/stz/observer_ptr.hpp>

namespace ui
{
    renderer::~renderer() { qDebug() << "DELETE renderer"; }


    void renderer::display(web_page* p)
    {
        static_cast<web_renderer*>(this)->display(p);
    }

    void renderer::display(widget_page* p)
    {
        static_cast<widget_renderer*>(this)->display(p);
    }

    ui::renderer* renderer::make(stz::observer_ptr<page> p)
    {
        return p->make_renderer();
    }
} // ui