#ifndef INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI
#define INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI

#include <nxi/command/fwd.hpp>

#include <QString>

namespace nxi
{
	class command_parameter
    {
    public:
        QString name;
        nxi::command_function_parameters_type suggestion_callback;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI
