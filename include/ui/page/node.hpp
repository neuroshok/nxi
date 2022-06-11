#ifndef INCLUDE_UI_PAGE_NODE_HPP_NXI
#define INCLUDE_UI_PAGE_NODE_HPP_NXI

#include <nds/graph/node.hpp>

#include <ui/page.hpp>
#include <QScopedPointer>
#include <QRect>

class QPaintEvent;
class QTimerEvent;
class QWebEnginePage;
class QWebEngineView;

namespace nxi { class page_node; }

namespace ui
{
    class user;
    class renderer;
    class web_page;

    class node_page : public ui::page
    {
        Q_OBJECT
    public:
        node_page(ui::user&, nds::node_ptr<const nxi::page> page);

        void display(ui::renderer* renderer) override;
        ui::renderer* make_renderer() const override;

        QWidget* widget();

    protected:
        void paintEvent(QPaintEvent*) override;
        void timerEvent(QTimerEvent*) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

    private:
        ui::user& user_;
        nds::node_ptr<const nxi::page> page_;

        QWebEngineView* view_;
        std::vector<ui::web_page*> pages_;
        std::vector<QRect> page_rects_;
        int timer_id_;
        int hl_index_;
    };
} // ui

#endif // INCLUDE_UI_PAGE_NODE_HPP_NXI