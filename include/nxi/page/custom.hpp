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
        custom_page(nxi::page_system& ps, QString name = "custom_page", QString command = "", nxi::renderer_type renderer_type = nxi::renderer_type::widget)
            : nxi::page(ps, std::move(name), std::move(command), nxi::page_type::custom, renderer_type)
            , renderer_type_{ renderer_type }
        {}

        void focus() { page_system_.focus(*this); }

    private:
        nxi::renderer_type renderer_type_;

    };
} // nxi

#endif // NXI_PAGE_CUSTOM_H_NXI
