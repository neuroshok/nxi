#ifndef INCLUDE_NXI_SYSTEM_INTERFACE_HPP_NXI
#define INCLUDE_NXI_SYSTEM_INTERFACE_HPP_NXI

#include <nxi/theme.hpp>

#include <QObject>

namespace nxi
{
    class core;
    class theme;

    class interface_system : public QObject
    {
        Q_OBJECT
    public:
		interface_system(nxi::core&);
		interface_system(const interface_system&) = delete;
        void operator=(const interface_system&) = delete;

        void load();
        void load_theme(const QString& name);
        nxi::theme& theme();

        signals:
        void event_load_theme(const nxi::theme&);

    private:
		nxi::core& nxi_core_;

		nxi::theme theme_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_INTERFACE_HPP_NXI
