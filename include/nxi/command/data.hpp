#ifndef INCLUDE_NXI_COMMAND_DATA_HPP_NXI
#define INCLUDE_NXI_COMMAND_DATA_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/shortcut.hpp>

#include <QString>

namespace nxi
{
	struct command_data
    {
	    QString module = "nxi";
		QString action = "none";
		nxi::command_function_type function;
        QString icon;
        QString description;
        nxi::shortcut shortcut;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_DATA_HPP_NXI
