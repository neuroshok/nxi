#include <ui/renderer/web.hpp>

#include <ui/page/web.hpp>

#include <QWebEngineView>
#include <include/nxw/hbox_layout.hpp>

namespace ui
{
    web_renderer::web_renderer()
    {
        view_ = new QWebEngineView(this);

        auto l = new nxw::hbox_layout;
        setLayout(l);
        l->addWidget(view_);
    }

    void web_renderer::display(web_page* page)
    {
        view_->setPage(page->native());
    }

    nxi::renderer_type web_renderer::type() const
    {
        return nxi::renderer_type::web;
    }
} // ui