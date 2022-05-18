#ifndef INCLUDE_UI_SYSTEM_INTERFACE_HPP_NXI
#define INCLUDE_UI_SYSTEM_INTERFACE_HPP_NXI

#include <QObject>
#include <QPixmap>
#include <QString>

#include <unordered_map>

namespace ui
{
    class core;

    class interface_system : public QObject
    {
        Q_OBJECT
    public:
        explicit interface_system(ui::core&);

        void add_icon(const QString& name, QPixmap icon);
        const QPixmap& icon(const QString& name);

    private:
        ui::core& ui_core_;

        std::unordered_map<QString, QPixmap> icons_;
    };
} // ui

#endif // INCLUDE_UI_SYSTEM_INTERFACE_HPP_NXI