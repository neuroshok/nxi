#ifndef INCLUDE_UI_MENU_HPP_NXI
#define INCLUDE_UI_MENU_HPP_NXI

#include <QFrame>


#include <QLabel>
#include <nxw/hbox_layout.hpp>
#include <nxw/vbox_layout.hpp>
#include <functional>
#include <nxi/command.hpp>

namespace nxw
{
    class vbox_layout;
    class hbox_layout;
}

namespace ui
{
    class core;

    class menu_separator : public QFrame
    {
    Q_OBJECT
    public:
        menu_separator()
        {
            setFrameStyle(QFrame::HLine);
            setFixedHeight(12);
        }
    };

    class menu_item : public QFrame
    {
        Q_OBJECT

        Q_PROPERTY(int height READ height WRITE height_set DESIGNABLE true)

    public:
        menu_item(const QString& str_name, std::function<void()> command = {}, const QString& str_icon = "") :
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

        void mouseReleaseEvent(QMouseEvent* event) override
        {
            if (command_) command_();
        }

        int height() const { return height_; }
        void height_set(int h) { height_ = h; setFixedHeight(height_); }

    private:
        int height_;
        std::function<void()> command_;
    };

    class menu : public QWidget
    {
        Q_OBJECT

    public:
        menu(QWidget* parent);

        void clear();

        void set_max_items(size_t n);

        template<class Widget, class... Args>
        void add(Args&&... args)
        {
            auto widget = new Widget(std::forward<Args>(args)...);
            item_layout_->addWidget(widget);
        }

        void add(const nxi::command& command);
        void add(const QString& action_name, std::function<void()> function);
        void add(QWidget* widget);
        void add_bottom(QWidget* widget);

        void focus_next();

        size_t item_count() const;
        void item_previous();
        void item_next();

        void show_at(QWidget* widget);

        void exec();

    private:
        //ui::core& ui_core;
        //std::vector<nxw::menu_item>
        int item_index_;
        nxw::vbox_layout* item_layout_;

        nxw::hbox_layout* bottom_layout_;
        QWidget* widget_origin_;

        size_t max_items_;

    };
} // nxw

#endif // INCLUDE_UI_MENU_HPP_NXI