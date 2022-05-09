#ifndef INCLUDE_NXI_NOTIFICATION_DATA_HPP_NXI
#define INCLUDE_NXI_NOTIFICATION_DATA_HPP_NXI

#include <nxi/type.hpp>
#include <nxi/utility.hpp>

#include <QString>

namespace nxi
{
    struct notification_data
    {
        nxi::notification_type type = nxi::notification_type::system;
        QString source = "nxi";
        QString title;
        QString message;

        template<class T>
        static T to_string(nxi::notification_type type)
        {
            switch (type)
            {
            case notification_type::module:
                return T{ "module" };
            case notification_type::system:
                return T{ "system" };
            case notification_type::web:
                return T{ "web" };
            default:
                nxi_unreachable();
            }
        }
    };
} // nxi

#endif // INCLUDE_NXI_NOTIFICATION_DATA_HPP_NXI