#include <nxi/page.hpp>

#include <nxi/database.hpp>
#include <nxi/log.hpp>

#include <nxi/system/page.hpp>

namespace nxi
{
    page::page(page_system& ps, QString name, QString command, nxi::page_type type, nxi::renderer_type renderer_type)
        : page_system_{ ps }
    {
        ndb_object::name = std::move(name);
        ndb_object::command = std::move(command);
        ndb_object::type = std::move(type);
        ndb_object::renderer_type = std::move(renderer_type);
    }

    void page::name_update(const QString& name)
    {
        ndb_object::name = name;

        //ndb::query<dbs::core>() << (ndb::set(nxi_model.page.name = name_) << ndb::filter(nxi_model.page.id == id_));

        emit event_update_name(ndb_object::name);
    }

    void page::command_update(const QString& command)
    {
        ndb_object::command = command;

        //emit event_command_update(ndb_object::name);
    }

    void page::load()
    {
        emit event_load();
    }

    void page::focus()
    {
        emit page_system_.event_focus(*this);
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

    const QString& page::command() const
    {
        return ndb_object::command;
    }

    nxi::page_type page::type() const
    {
        return ndb_object::type;
    }

    nxi::renderer_type page::renderer_type() const
    {
        return ndb_object::renderer_type;
    }
} // nxi