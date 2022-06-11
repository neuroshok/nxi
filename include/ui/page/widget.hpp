#ifndef UI_PAGE_WIDGET_H_NXI
#define UI_PAGE_WIDGET_H_NXI

#include <ui/page.hpp>

namespace nxi { class custom_page; }

namespace ui
{
    class user;
    class renderer;

    class widget_page : public ui::page
    {
        Q_OBJECT
    public:
        widget_page(ui::user&, nxi::custom_page& page);

        void display(ui::renderer* renderer) override;
        ui::renderer* make_renderer() const override;

        QWidget* widget();

    private:
        ui::user& user_;
        nxi::custom_page& page_;
        QWidget* widget_;
    };
} // ui

#endif // UI_PAGE_WIDGET_H_NXI