#ifndef NXI_PAGE_CUSTOM_H_NXI
#define NXI_PAGE_CUSTOM_H_NXI

#include <nxi/page.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    class page_system;

    class custom_page : public nxi::page
    {
        Q_OBJECT
    public:
        custom_page(nxi::page_system& ps, nxi::page_id id)
            : nxi::page(ps, id, "custom_page")
        {}

        custom_page(nxi::page_system& ps, nxi::page_id id, const QString& name) : nxi::page(ps, id, name) {}

        void focus() { page_system_.focus(*this); }

        static nxi::page_type type() { return page_type::custom; }

    private:

    };
} // nxi

#endif // NXI_PAGE_CUSTOM_H_NXI
