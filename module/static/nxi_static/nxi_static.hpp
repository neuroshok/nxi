#ifndef EXP_MODULE_TEST_H_NXI
#define EXP_MODULE_TEST_H_NXI

#include <nxi/module.hpp>

namespace nxi { class core; }

namespace nxi::modules
{
    class nxi_static : public nxi::module
    {
    public:
        explicit nxi_static(nxi::core& nxi_core);

        void on_load();

    private:
        nxi::core& nxi_core_;

    };
} // nxi::modules

#endif // EXP_MODULE_TEST_H_NXI