#ifndef INCLUDE_NXI_SYSTEM_INTERFACE_HPP_NXI
#define INCLUDE_NXI_SYSTEM_INTERFACE_HPP_NXI

#include <nxi/style.hpp>

#include <QObject>

namespace nxi
{
    class core;
    class style;

    class interface_system : public QObject
    {
        Q_OBJECT
    public:
		interface_system(nxi::core&);
		interface_system(const interface_system&) = delete;
        void operator=(const interface_system&) = delete;

        void load();
        void load_style(const QString& name);
        nxi::style& style();

        signals:
        void event_update_style(const nxi::style&);

    private:
		nxi::core& nxi_core_;

		nxi::style style_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_INTERFACE_HPP_NXI
