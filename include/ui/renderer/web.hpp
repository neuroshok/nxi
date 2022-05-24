#ifndef UI_RENDERER_WEB_H_NXI
#define UI_RENDERER_WEB_H_NXI

#include <ui/renderer.hpp>

#include <QPainter>
#include <QWidget>

class QFocusEvent;
class QWebEngineView;

#include <QDebug>
#include <QWebEngineView>

namespace ui
{
    class web_page;

    class web_renderer : private QWebEngineView, public ui::renderer
    {
        Q_OBJECT
    public:
        web_renderer();

        void display(web_page* p);
        [[nodiscard]] nxi::renderer_type type() const override;
        QWidget* widget() override { return this; }

    signals:
        void event_focus();

    protected:
        void focusInEvent(QFocusEvent*) override;
        void focusOutEvent(QFocusEvent*) override;

    private:
        ui::web_page* web_page_;
    };
} // ui

#endif // UI_RENDERER_WEB_H_NXI