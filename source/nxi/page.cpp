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
    {
        ndb_object::name = std::move(name);
        ndb_object::command = std::move(command);
        ndb_object::type = std::move(type);
        ndb_object::renderer_type = std::move(renderer_type);
        ndb_object::loaded = false;
        ndb_object::muted = false;
    }

    void page::update_name(const QString& name)
    {
        ndb_object::name = name;
        ndb::store(*this);
        emit event_update_name(ndb_object::name);
    }

    void page::update_command(const QString& command)
    {
        ndb_object::command = command;
        ndb::store(*this);
        emit event_update_command(ndb_object::command);
        emit page_system_.event_update_command(node_ptr_);
    }

    void page::update_icon(const QIcon& icon)
    {
        icon_ = icon;
    }

    void page::close()
    {
        emit event_close();
        page_system_.close(node_ptr_);
        ndb::unload(*this);
    }

    void page::focus()
    {
        if (!is_loaded()) load();
        emit event_focus();
        emit page_system_.event_focus(node_ptr_);
    }

    void page::load()
    {
        ndb_object::loaded = true;
        ndb::store(*this);
        emit event_load();
        emit page_system_.event_load(node_ptr_);
    }

    void page::run_script(const QString& script) const
    {
        emit event_run_script(script);
    }

    nxi::page_id page::id() const
    {
        nxi_assert(ndb::is_valid(*this));
        return ndb_object::oid;
    }

    const QString& page::name() const
    {
        return ndb_object::name;
    }

    const QString& page::command() const { return ndb_object::command; }
    bool page::is_loaded() const { return ndb_object::loaded; }
    bool page::is_muted() const { return ndb_object::muted; }
    nxi::page_type page::type() const { return ndb_object::type; }
    nxi::renderer_type page::renderer_type() const { return ndb_object::renderer_type; }
    const QIcon& page::icon() const { return icon_; }
} // nxi