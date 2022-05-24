#include <ui/renderer/web.hpp>

#include <nxi/page/web.hpp>

#include <ui/page/web.hpp>

#include <QCoreApplication>
#include <QFocusEvent>

namespace ui
{
    web_renderer::web_renderer()
        : web_page_{ nullptr }
    {}

    void web_renderer::display(ui::web_page* page)
    {
        setPage(page->native());
        web_page_ = page;
    }

    nxi::renderer_type web_renderer::type() const { return nxi::renderer_type::web; }

    void web_renderer::focusInEvent(QFocusEvent* event)
    {
        if (web_page_) web_page_->nxi_page().focus();
        if (parent()) QCoreApplication::sendEvent(parent(), event);
    }

    void web_renderer::focusOutEvent(QFocusEvent* event)
    {
        if (parent()) QCoreApplication::sendEvent(parent(), event);
    }
} // ui