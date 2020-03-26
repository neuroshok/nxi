#ifndef NXI_TYPE_H_NXI
#define NXI_TYPE_H_NXI

namespace nxi
{
    using page_id = unsigned int;

    enum class renderer_type { web, widget };
    enum class page_type { node, static_, web, explorer, custom };
    enum class module_type { compiled, dynamic, web };

} // nxi

#endif // NXI_TYPE_H_NXI