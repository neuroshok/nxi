#ifndef NXI_PAGE_H_NXI
#define NXI_PAGE_H_NXI

#include <nxi/database.hpp>
#include <nxi/type.hpp>

#include <ndb/object.hpp>
#include <nds/graph/node.hpp>

#include <QIcon>
#include <QObject>

class QString;

namespace nxi
{
    class page_system;

    class page : public QObject, public ndb::object<dbs::core, ndb::objects::page>
    {
        using ndb_object = ndb::object<dbs::core, ndb::objects::page>;

        Q_OBJECT
    public:
        virtual void close();
        virtual void focus();
        virtual void load();
        virtual void run_script(const QString& script) const;

        void update_command(const QString& command);
        void update_icon(const QIcon&);
        void update_name(const QString& name);

        nxi::page_id id() const;
        const QString& name() const;
        const QString& command() const;
        bool is_loaded() const;
        bool is_muted() const;
        nxi::page_type type() const;
        nxi::renderer_type renderer_type() const;
        const QIcon& icon() const;

        page(nds::node_ptr<nxi::page>, page_system& ps
            , QString name = "new_page", QString command = ""
            , nxi::page_type = nxi::page_type::custom
            , nxi::renderer_type = nxi::renderer_type::web);

    protected:
        nxi::page_system& page_system_;

    private:
        QIcon icon_;
        nds::node_ptr<nxi::page> node_ptr_;

    signals:
        void event_update_command(const QString&);
        void event_update_icon(const QIcon&);
        void event_update_name(const QString&);

        void event_close();
        void event_focus();
        void event_load();
        void event_run_script(const QString&) const;
    };
} // nxi

#endif // NXI_PAGE_H_NXI