#include <nxw/menu/item.hpp>
#include <nxw/hbox_layout.hpp>

#include <QFrame>
#include <QLabel>
#include <QVariant>
#include <QStyle>
#include <QDebug>

namespace nxw
{
    menu_item::menu_item(const QString& str_name, std::function<void()> command, const QString& str_icon) :
        command_{ std::move(command) }
    {
        auto layout_ = new nxw::hbox_layout;
        setLayout(layout_);


        auto icon = new QLabel(this);
        if (!str_icon.isEmpty())
        {
            icon->setPixmap(QPixmap(str_icon).scaledToWidth(16));
        }
        icon->setFixedWidth(24);

        auto label = new QLabel(this);
        label->setText(str_name);

        layout_->addWidget(icon);
        layout_->addWidget(label);
        layout_->addStretch(1);
    }

    void menu_item::set_state(states state)
    {
        switch (state)
        {
            case states::idle: setProperty("state", QVariant::fromValue(QString("idle"))); break;
            case states::hover: setProperty("state", QVariant::fromValue(QString("hover"))); break;
            case states::selected: setProperty("state", QVariant::fromValue(QString("selected"))); break;
        }

        style()->unpolish(this);
        style()->polish(this);
    }

    void menu_item::mouseReleaseEvent(QMouseEvent* event)
    {
        if (command_) command_();
        QWidget::mouseReleaseEvent(event);
    }

    void menu_item::enterEvent(QEvent *event)
    {

        //ui::theme_system::apply<theme_system::w3c::icons>("")
        //set_state(states::hover);
        QWidget::enterEvent(event);
    }

    void menu_item::leaveEvent(QEvent *event)
    {

        QWidget::leaveEvent(event);
    }
} // nxw
