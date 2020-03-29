#include <ui/page/node.hpp>

#include <nxi/page/node.hpp>
#include <nxi/core.hpp>

#include <ui/core.hpp>
#include <ui/renderer/widget.hpp>
#include <ui/page/web.hpp>
#include <nxw/hbox_layout.hpp>

#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineSettings>

// todo experimental / optimize

namespace ui
{
    node_page::node_page(ui::core& ui_core, nds::node_ptr<const nxi::page> page)
        : ui::page{ const_cast<nxi::page&>(*page) } // tmp
        , page_{ std::move(page) }
        , ui_core_{ ui_core }
    {
        view_ = new QWebEngineView;
        view_->setAttribute(Qt::WA_DontShowOnScreen);
        view_->show();
        //timer_id_ = startTimer(1000);
        setMouseTracking(true);
    }

    void node_page::display(ui::renderer *renderer)
    {
        renderer->display(this);
    }

    ui::renderer* node_page::make_renderer() const
    {
        return new widget_renderer;
    }

    QWidget* node_page::widget() { return this; }

    void node_page::timerEvent(QTimerEvent* event)
    {
        if (event->timerId() == timer_id_) update();
    }

    void node_page::mouseMoveEvent(QMouseEvent* event)
    {
        /*
        auto previous_i = hl_index_;
        int page_width = width() / pages_.size();

        for (int i = 0; i < pages_.size(); ++i)
        {
            QRect r = page_rects_[i];
            if (QRect(mapToGlobal(r.topLeft()), r.size()).contains(event->globalPos()))
            {
                hl_index_ = i;
                if (hl_index_ != previous_i) repaint();
                return;
            }
        }
        hl_index_ = -1;*/
    }

    void node_page::paintEvent(QPaintEvent*)
    {

        auto pages = ui_core_.nxi_core().page_system().targets(page_);
        int page_x = 0;
        int page_y = 0;
        int page_width = width() / pages.size();

        view_->setFixedSize(page_width, height());

        page_rects_.clear();
        pages_.clear();

        for (auto p : pages)
        {
            page_rects_.emplace_back(page_x, 0, page_width, height());
            auto ui_page = ui_core_.page_system().get(*p).get();
            pages_.push_back(static_cast<ui::web_page*>(ui_page));
            page_x += page_width;
        }

        for (int i = 0; i < pages_.size(); ++i)
        {
            view_->setPage(pages_[i]->native());
            view_->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
            view_->render(this, QPoint{page_rects_[i].topLeft()} , QRegion{});


            QPainter painter(this);
            QRect b = page_rects_[i];
            b.setTop(height() / 10);
            b.setBottom(height() / 10);
            painter.fillRect(b, QColor(0, 0, 0, 80));
        }
    }

    void node_page::mouseReleaseEvent(QMouseEvent *event)
    {
        for (int i = 0; i < pages_.size(); ++i)
        {
            QRect r = page_rects_[i];
            if (QRect(mapToGlobal(r.topLeft()), r.size()).contains(event->globalPos()))
            {
                ui_core_.nxi_core().page_system().focus(pages_[i]->nxi_page().id());
                break;
            }
        }
    }

    void node_page::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);


    }
} // ui