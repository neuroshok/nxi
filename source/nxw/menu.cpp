//! \brief A widget to create custom menus with actions

#include <nxw/menu.hpp>
#include <nxw/menu/item.hpp>
#include <nxw/vbox_layout.hpp>
#include <nxw/hbox_layout.hpp>

#include <QLabel>
#include <QEvent>
#include <QStyle>
#include <QVariant>
#include <QDebug>

#include <nxi/log.hpp>

namespace nxw
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
        top_layout_ = new nxw::hbox_layout;

        auto main_layout = new nxw::vbox_layout;
        main_layout->addLayout(top_layout_);
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


    void menu::add(const QString& action_name, std::function<void()> function)
    {
        add<nxw::menu_item>(action_name, std::move(function));
    }

    void menu::add(QWidget* widget)
    {
        item_layout_->addWidget(widget);
    }

    void menu::add_bottom(QWidget* widget)
    {
        bottom_layout_->addWidget(widget);
    }

    void menu::add_top(QWidget* widget)
    {
        top_layout_->addWidget(widget);
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
        setFixedHeight(item_count() * style_data.item_height);
        //show();
    }

    void menu::select_previous()
    {
        if (!item_count()) return;
        if (item_index_ < 0) item_index_ = 0;
        //item_at(item_index_)->set_state(menu_item::states::idle);
        item_previous();
        //item_at(item_index_)->set_state(menu_item::states::selected);
    }
    void menu::select_next()
    {
        if (!item_count()) return;
        if (item_index_ < 0) item_index_ = item_count() - 1;
        //item_at(item_index_)->set_state(menu_item::states::idle);
        item_next();
        //item_at(item_index_)->set_state(menu_item::states::selected);
    }


    size_t menu::item_count() const
    {
        return item_layout_->count();
    }

    void menu::item_previous()
    {
        if (!item_count()) return;
        item_index_ = (item_index_ + item_count() - 1) % item_count();
    }

    void menu::item_next()
    {
        if (!item_count()) return;
        item_index_ = ++item_index_ % item_count();
    }

    nxw::menu_item* menu::item_at(size_t index) const
    {
        nxi_assert(index < item_count());
        return static_cast<nxw::menu_item*>(item_layout_->itemAt(index)->widget()) ;
    }
} // nxw
