#ifndef INCLUDE_NXW_MENU_HPP_NXI
#define INCLUDE_NXW_MENU_HPP_NXI

#include <functional>

#include <QWidget>

class QString;

namespace nxw
{
    class menu_item;
    class hbox_layout;
    class vbox_layout;

    class menu : public QWidget
    {
        Q_OBJECT
    public:
        menu(QWidget* parent);

        void clear();

        void set_max_items(size_t n);

        template<class Widget, class... Args>
        void add(Args&&... args);
        void add(const QString& action_name, std::function<void()> function);
        void add(QWidget* widget);
        void add_bottom(QWidget* widget);

        void select_previous();
        void select_next();

        nxw::menu_item* item_at(size_t index) const;
        size_t item_count() const;
        void item_previous();
        void item_next();

        void show_at(QWidget* widget);

        void exec();

    private:
        int item_index_;
        nxw::vbox_layout* item_layout_;

        nxw::hbox_layout* bottom_layout_;
        QWidget* widget_origin_;

        size_t max_items_;

    };
} // nxw

namespace nxw
{
    template<class Widget, class... Args>
    void menu::add(Args&&... args)
    {
        auto widget = new Widget(std::forward<Args>(args)...);
        item_layout_->addWidget(widget);
    }
} // nxw

#endif // INCLUDE_NXW_MENU_HPP_NXI
