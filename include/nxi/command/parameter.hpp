#ifndef INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI
#define INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI

#include <nxi/command/fwd.hpp>

#include <QString>

namespace nxi
{
    class command_parameter
    {
    public:
        command_parameter(QString name, nxi::command_function_parameters_type suggestion_callback = nullptr);

        const QString& name() const;
        void suggestion_callback(nxi::suggestion_vector& v) const;

    private:
        QString name_;
        nxi::command_function_parameters_type suggestion_callback_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_PARAMETER_HPP_NXI
