#include <ui/renderer/widget.hpp>

#include <QDebug>
#include <QLineEdit>
#include <ui/page/widget.hpp>
#include <ui/page/node.hpp>
#include <QHBoxLayout>

namespace ui
{
    widget_renderer::widget_renderer()
    {
        layout_ = new QHBoxLayout;
        setLayout(layout_);
    }

    void widget_renderer::display(node_page* p)
    {
        layout_->addWidget(p);
    }

    void widget_renderer::display(widget_page* p)
    {
        layout_->addWidget(p->widget());
    }

    nxi::renderer_type widget_renderer::type() const
    {
        return nxi::renderer_type::widget;
    }
} // ui