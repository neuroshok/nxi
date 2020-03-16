#ifndef INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI
#define INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI

#include <QString>

namespace nxi
{
	struct command_parameter
    {
        QString name;
        nxi::command_function_parameters_type suggestion_callback;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI
