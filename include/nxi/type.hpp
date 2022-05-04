#ifndef NXI_TYPE_H_NXI
#define NXI_TYPE_H_NXI

namespace nxi
{
    using page_id = unsigned int;

    enum class context_type
    {
        command
        , command_executor
        , custom
        , page
    };
    enum class module_type
    {
        compiled
        , dynamic
        , web
    };
    enum class page_type
    {
        node
        , static_
        , web
        , explorer
        , custom
    };
    enum class renderer_type
    {
        web
        , widget
    };
    enum class session_type
    {
        basic
        , user
    };
} // nxi

#endif // NXI_TYPE_H_NXI