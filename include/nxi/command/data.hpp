#ifndef INCLUDE_NXI_COMMAND_DATA_HPP_NXI
#define INCLUDE_NXI_COMMAND_DATA_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/shortcut.hpp>

#include <QString>

namespace nxi
{
	struct command_data
    {
	    struct parameter
        {
	        QString name;
	        nxi::command_function_parameters_type parameters;
        };

	    QString module = "nxi";
		QString action = "none";
		nxi::command_function_type function;
		std::vector<parameter> parameters;
        QString icon;
        QString description;
        nxi::shortcut shortcut;
        bool preview = false;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_DATA_HPP_NXI
