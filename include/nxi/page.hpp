#ifndef NXI_PAGE_H_NXI
#define NXI_PAGE_H_NXI

#include <nxi/database.hpp>
#include <nxi/data/page.hpp>
#include <nxi/type.hpp>

#include <nds/graph/node.hpp>

#include <QIcon>
#include <QObject>

class QString;
class QWebEngineScript;

namespace nxi
{
    class page_system;

    class page : public QObject
    {
        friend nxi::page_id nxi::data::page::add(nxi::database&, nxi::page&);

        Q_OBJECT
    public:
        virtual void run_script(const QString& script) const;

        void set_loaded();
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
        nxi::page_id id_;
        QString name_;
        QString command_;
        nxi::page_type type_;
        nxi::renderer_type renderer_type_;
        bool is_loaded_;
        bool is_muted_;
        QIcon icon_;

        nds::node_ptr<nxi::page> node_ptr_;

    signals:
        void event_update_command(const QString&);
        void event_update_icon(const QIcon&);
        void event_update_name(const QString&);

        void event_close();
        void event_focus();
        void event_load();
    };
} // nxi

#endif // NXI_PAGE_H_NXI