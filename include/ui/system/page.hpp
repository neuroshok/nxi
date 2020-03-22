#ifndef UI_SYSTEM_PAGE_H_NXI
#define UI_SYSTEM_PAGE_H_NXI

#include <nxi/log.hpp>
#include <nxi/type.hpp>

#include <unordered_map>

#include <QObject>
#include <QHash>
#include <QPointer>
#include <QString>
#include <stz/observer_ptr.hpp>
#include <ui/page.hpp>

class QWidget;

namespace nxi { class page; }

namespace ui
{
    class core;
    class page;

    class page_system : public QObject
    {
        Q_OBJECT
    public:
        page_system(ui::core& ui_core);
        ~page_system();

        QWidget* get(const QString& page_path);
        stz::observer_ptr<ui::page> get(const nxi::page&);

        template<class Widget>
        auto make_widget(const QString& path)
        {
            auto it = widget_pages_.find(path);
            if (it != widget_pages_.end()) nxi_warning("page {} already exists", path);
            else
            {
                auto widget = new Widget(ui_core_);
                widget_pages_.insert(path, widget);
            }
        }

    private:
		ui::core& ui_core_;
        QHash<QString, QWidget*> widget_pages_;
        std::unordered_map<nxi::page_id, QPointer<ui::page>> pages_;
    };
} // ui

#endif // UI_SYSTEM_PAGE_H_NXI