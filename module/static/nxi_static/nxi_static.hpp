#ifndef EXP_MODULENXI_STATIC_TEST_H_NXI
#define EXP_MODULENXI_STATIC_TEST_H_NXI

#include <nxi/module.hpp>

namespace nxi { class session; }

namespace nxi::modules
{
    class nxi_static : public nxi::module
    {
    public:
        explicit nxi_static(nxi::session&);

        void on_load();

    private:
        nxi::session& session_;

    };
} // nxi::modules

#endif // EXP_MODULENXI_STATIC_TEST_H_NXI