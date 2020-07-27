#ifndef UI_PAGE_H_NXI
#define UI_PAGE_H_NXI

#include <QWidget>

#include <nxi/page.hpp>

namespace ui
{
    class renderer;

    class page : public QWidget
    {
    public:
        page(nxi::page& page) : page_{ page } {}
        virtual ~page() = default;

        virtual void display(renderer*) = 0;
        virtual ui::renderer* make_renderer() const = 0;

        nxi::page& nxi_page() { return page_; }

    private:
        nxi::page& page_;
    };
} // ui

#endif // UI_PAGE_H_NXI