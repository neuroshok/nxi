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

    QWidget* widget_renderer::widget()
    {
        return this;
    }

} // ui