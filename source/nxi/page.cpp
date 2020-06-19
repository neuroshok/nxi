#include <nxi/page.hpp>

#include <nxi/log.hpp>
#include <nxi/system/page.hpp>

#include <QString>
#include <QDebug>

namespace nxi
{
    page::page(nds::node_ptr<nxi::page> node_ptr, page_system& ps, QString name, QString command, nxi::page_type type, nxi::renderer_type renderer_type)
        : page_system_{ ps }
        , node_ptr_{ std::move(node_ptr) }
        , name_{ std::move(name) }
    {

    }

    void page::update_name(const QString& name)
    {
        // ndb::store(*this);
        //emit event_update_name(name);
    }

    void page::update_command(const QString& command)
    {

        // ndb::store(*this);
        //emit event_update_command(command);
        emit page_system_.event_update_command(node_ptr_);
    }

    void page::update_icon(const QIcon& icon)
    {
        icon_ = icon;
    }


    void page::set_loaded()
    {
        //loaded = true;
        // ndb::store(*this);
    }

    void page::run_script(const QString& script) const
    {
        nxi_warning("unimplemented");
    }

    nxi::page_id page::id() const
    {
        //nxi_assert(ndb::is_valid(*this));
        return id_;
    }

    const QString& page::name() const { return name_; }
    const QString& page::command() const { return command_; }
    bool page::is_loaded() const { return is_loaded_; }
    bool page::is_muted() const { return is_muted_; }
    nxi::page_type page::type() const { return type_; }
    nxi::renderer_type page::renderer_type() const { return renderer_type_; }
    const QIcon& page::icon() const { return icon_; }
} // nxi