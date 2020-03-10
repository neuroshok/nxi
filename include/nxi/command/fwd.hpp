#ifndef INCLUDE_NXI_COMMAND_FWD_HPP_NXI
#define INCLUDE_NXI_COMMAND_FWD_HPP_NXI

namespace nds { template<class> class node_ptr; }

#include <functional>
#include <vector>

namespace nxi
{
    class command;
    class command_params;
    class suggestion_vector;

    using commands_view = std::vector<nds::node_ptr<const nxi::command>>;
    using command_function_type = std::function<void(const nxi::command_params&)>;
    using command_function_parameters_type = std::function<void(nxi::suggestion_vector&)>;
} // nxi

#endif // INCLUDE_NXI_COMMAND_FWD_HPP_NXI
