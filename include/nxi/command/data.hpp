#ifndef INCLUDE_NXI_COMMAND_DATA_HPP_NXI
#define INCLUDE_NXI_COMMAND_DATA_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/command/parameter.hpp>
#include <nxi/context.hpp>
#include <nxi/shortcut.hpp>

#include <QString>

namespace nxi
{
    struct command_data
    {
        QString module = "nxi";
        QString action = "none";
        nxi::command_function_type function;
        std::vector<nxi::command_parameter> parameters;
        QString icon;
        QString description;
        nxi::shortcut shortcut;
        nxi::context_id context_id;
        bool preview = false;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_DATA_HPP_NXI