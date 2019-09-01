#ifndef UI_RENDERER_WEB_H_NXI
#define UI_RENDERER_WEB_H_NXI

#include <ui/renderer.hpp>

#include <QWidget>
#include <QPainter>

class QWebEngineView;

#include <QWebEngineView>
#include <QDebug>

namespace ui
{
    class web_page;

    class web_renderer : public ui::renderer
    {
    public:
        web_renderer();

        void display(web_page* p);
        nxi::renderer_type type() const override;

    private:
        QWebEngineView* view_;
    };
} // ui

#endif // UI_RENDERER_WEB_H_NXI