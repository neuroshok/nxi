#ifndef INCLUDE_NXI_COMMAND_FWD_HPP_NXI
#define INCLUDE_NXI_COMMAND_FWD_HPP_NXI

#include <stz/observer_ptr.hpp>
#include <vector>

namespace nxi
{
    class command;
    class command_params;

    using commands_view = std::vector<stz::observer_ptr<nxi::command>>;
    using command_function_type = std::function<void(const nxi::command_params&)>;
} // nxi

#endif // INCLUDE_NXI_COMMAND_FWD_HPP_NXI
