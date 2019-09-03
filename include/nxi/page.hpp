#ifndef NXI_PAGE_H_NXI
#define NXI_PAGE_H_NXI

#include <nxi/page/id.hpp>
#include <nxi/type.hpp>

#include <ndb/object.hpp>
#include <nxi/database.hpp>

#include <QObject>
#include <QString>

namespace nxi
{
    class page_system;

    class page : public QObject, public ndb::object<dbs::core, ndb::objects::page>
    {
        using ndb_object = ndb::object<dbs::core, ndb::objects::page>;

        Q_OBJECT
    public:
        nxi::page_id id() const;
        const QString& name() const;
        const QString& command() const;
        nxi::page_type type() const;
        nxi::renderer_type renderer_type() const;

        virtual void focus();
        virtual void load();

        void name_update(const QString& name);
        void command_update(const QString& command);

        page(page_system& ps, QString name = "new_page", QString command = "", nxi::page_type = nxi::page_type::custom, nxi::renderer_type = nxi::renderer_type::web);
    protected:


        nxi::page_system& page_system_;

    private:


    signals:
        void event_update_name(const QString&);
        void event_update_icon(const QIcon&);
        void event_load();
    };
} // nxi

#endif // NXI_PAGE_H_NXI