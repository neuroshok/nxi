#include <ui/menu.hpp>

#include <nxi/log.hpp>

#include <nxw/vbox_layout.hpp>
#include <nxw/hbox_layout.hpp>

#include <ui/interface.hpp>

#include <QLabel>
#include <QEvent>
#include <QStyle>

#include <nxi/command.hpp>

namespace ui
{
    menu::menu(QWidget* parent)
        : QWidget(parent)
        , widget_origin_{ nullptr }
        , max_items_{ 5 }
        , item_index_{ -1 }
    {
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

        item_layout_ = new nxw::vbox_layout;
        item_layout_->setAlignment(item_layout_, Qt::AlignTop);

        bottom_layout_ = new nxw::hbox_layout;

        auto main_layout = new nxw::vbox_layout;
        main_layout->addLayout(item_layout_);
        main_layout->addLayout(bottom_layout_);
        setLayout(main_layout);
    }

    void menu::clear()
    {
        while (auto item = item_layout_->takeAt(0))
        {
            delete item->widget();
        }
        item_index_ = -1;
    }

    void menu::set_max_items(size_t n) { max_items_ = n; }

    void menu::add(const nxi::command& command)
    {
        add<ui::menu_item>(command.name(), command.function(), command.icon());
    }

    void menu::add(const QString& action_name, std::function<void()> function)
    {
        add<ui::menu_item>(action_name, std::move(function));
    }

    void menu::add(QWidget* widget)
    {
        item_layout_->addWidget(widget);
    }

    void menu::add_bottom(QWidget* widget)
    {
        bottom_layout_->addWidget(widget);
    }

    void menu::show_at(QWidget* widget)
    {
        widget_origin_ = widget;
    }

    void menu::exec()
    {
        QPoint position = QCursor::pos();

        if (widget_origin_)
        {
            // move menu to widget global position
            position = widget_origin_->mapToGlobal(widget_origin_->rect().bottomLeft());
        }

        move(position);
        show();
        adjustSize();
    }

    void menu::focus_previous()
    {
        if (item_index_ < 0) item_index_ = 0;
        auto item_prev = item_at(item_index_);
        item_prev->setProperty("selected", false);
        item_prev->style()->polish(item_prev);item_prev->style()->unpolish(item_prev);
        item_previous();
        auto item_next = item_at(item_index_);
        item_next->setProperty("selected", true);
        item_next->style()->polish(item_next);item_next->style()->unpolish(item_next);
    }
    void menu::focus_next()
    {
        if (item_index_ < 0) item_index_ = item_count() - 1;
        auto item_prev = item_at(item_index_);
        item_prev->setProperty("selected", false);
        item_prev->style()->polish(item_prev);item_prev->style()->unpolish(item_prev);
        item_next();
        auto item_next = item_at(item_index_);
        item_next->setProperty("selected", true);
        item_next->style()->polish(item_next);item_next->style()->unpolish(item_next);
    }

    size_t menu::item_count() const
    {
        return item_layout_->count();
    }

    void menu::item_previous()
    {
        item_index_ = (item_index_ + item_count() - 1) % item_count();
    }

    void menu::item_next()
    {
        item_index_ = ++item_index_ % item_count();
    }

    ui::menu_item* menu::item_at(size_t index) const
    {
        nxi_assert(index < static_cast<size_t>(item_layout_->count()));
        return static_cast<ui::menu_item*>(item_layout_->itemAt(index)->widget()) ;
    }
} // ui
