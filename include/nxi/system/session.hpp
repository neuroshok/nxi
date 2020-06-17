#ifndef INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI
#define INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI

#include <QObject>
#include <QString>

namespace nxi
{
    class core;

    class session_system : public QObject
    {
    public:
        session_system(nxi::core&);
        session_system(const session_system&) = delete;
        void operator=(const session_system&) = delete;

        void load();

    private:
        nxi::core& nxi_core_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_SESSION_HPP_NXI
