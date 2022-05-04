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
        enum class properties { color, custom };

        page(nds::node_ptr<nxi::page>, page_system& ps, nxi::page_data);
        page(nds::node_ptr<nxi::page>, page_system& ps
            , QString name = "new page", QString command = ""
            , nxi::page_type = nxi::page_type::custom
            , nxi::renderer_type = nxi::renderer_type::web);

        virtual void run_script(const QString& script) const;

        void load();
        void load(const QString& command);
        void close();

        void set_audible(bool state);
        void set_muted(bool state);

        void toggle_mute();

        void update_audible(bool state);
        void update_color(const QColor&);
        void update_command(const QString& command);
        void update_icon(const QIcon&);
        void update_mute(bool state);
        void update_name(const QString& name, bool override = false);
        void update_property(const QString& name, const QString& value);
        void update_session(int id);

        nxi::page_id id() const;
        const QString& name() const;
        const QString& command() const;
        nxi::page_type type() const;
        nxi::renderer_type renderer_type() const;
        int session_id() const;

        bool has_color() const;

        bool is_audible() const;
        bool is_loaded() const;
        bool is_muted() const;

        const QColor& color() const;
        const QIcon& icon() const;

    protected:
        nxi::page_system& page_system_;

    private:
        nxi::page_id id_;
        QString name_;
        QString command_;
        nxi::page_type type_;
        nxi::renderer_type renderer_type_;
        int session_id_;

        bool has_color_;
        bool has_name_;

        bool is_audible_;
        bool is_loaded_;
        bool is_muted_;

        QColor color_;
        QIcon icon_;

        // std::unordered_map<QString, QString> properties_;

        nds::node_ptr<nxi::page> node_ptr_;

    signals:
        void event_update_command(const QString&);
        void event_update_icon(const QIcon&);
        void event_update_mute(bool state);
        void event_update_name(const QString&);

        void event_close();
        void event_focus();
        void event_load();
    };
} // nxi

#endif // NXI_PAGE_H_NXI