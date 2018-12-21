#ifndef NXI_STATIC_MODULE_H_NXI
#define NXI_STATIC_MODULE_H_NXI

#include "module/strateon/strateon.hpp"
#include "module/test/test.hpp"


namespace nxi
{
	class core;

    class static_module_container
    {
    public:
		explicit static_module_container(nxi::core& nxi_core) :
			nxi_core_{ nxi_core }
            , strateon_{ nxi_core }
, test_{ nxi_core }

		{}

        void load()
		{
            strateon_.load();
test_.load();

		}

    private:
		nxi::core& nxi_core_;

        nxi::modules::strateon strateon_;
nxi::modules::test test_;

    };
} // nxi

#endif // NXI_STATIC_MODULE_H_NXI
