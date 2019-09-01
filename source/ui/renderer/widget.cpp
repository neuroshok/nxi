#include <ui/renderer/widget.hpp>

#include <QDebug>
#include <QLineEdit>
#include <ui/page/widget.hpp>
#include <QHBoxLayout>

namespace ui
{
    void widget_renderer::display(widget_page* p)
    {
        auto layout = new QHBoxLayout;
        setLayout(layout);
        layout->addWidget(p->widget());
    }

    nxi::renderer_type widget_renderer::type() const
    {
        return nxi::renderer_type::widget;
    }
} // ui