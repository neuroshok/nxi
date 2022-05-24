#include <nxi/page.hpp>

#include <nxi/log.hpp>
#include <nxi/system/page.hpp>

#include <QDebug>
#include <QString>

namespace nxi
{
    page::page(nds::node_ptr<nxi::page> node_ptr, page_system& ps, nxi::page_data data)
        : page_system_{ ps }
        , node_ptr_{ std::move(node_ptr) }
        , id_{ data.id }
        , name_{ std::move(data.name) }
        , command_{ std::move(data.command) }
        , type_{ data.type }
        , renderer_type_{ data.renderer_type }
        , has_color_{ false }
        , has_name_{ false }
        , is_audible_{ std::move(data.audible) }
        , is_loaded_{ std::move(data.loaded) }
        , is_muted_{ std::move(data.muted) }
        , session_id_{ std::move(data.session_id) }
    {}

    page::page(nds::node_ptr<nxi::page> node_ptr,
               page_system& ps,
               QString name,
               QString command,
               nxi::page_type type,
               nxi::renderer_type renderer_type)
        : nxi::page(std::move(node_ptr),
                    ps,
                    nxi::page_data{ 0, std::move(name), std::move(command), type, renderer_type, false, false, false, ps.session_id() })
    {}

    void page::update_audible(bool state)
    {
        is_audible_ = state;
        nxi::data::page::update(page_system_.user_database_, *this);
    }

    void page::update_color(const QColor& color) { color_ = color; }

    void page::update_command(const QString& command)
    {
        if (command_ == command) return;

        emit event_update_command(command_);
        emit page_system_.event_update_command(*this, command);
        command_ = command;
        nxi::data::page::update(page_system_.user_database_, *this);
    }

    void page::update_icon(const QIcon& icon) { icon_ = icon; }

    void page::update_name(const QString& name, bool override)
    {
        if (has_name_ && !override) return;
        name_ = name;
        nxi::data::page::update(page_system_.user_database_, *this);
        emit event_update_name(name_);
    }

    void page::update_property(const QString& name, const QString& value)
    {
        if (name == "color")
        {
            has_color_ = true;
            update_color(QColor{ value });
        }
        else if (name == "name")
        {
            has_name_ = true;
            update_name(value, true);
        }
        else nxi_warning("property {} not found", name);
    }

    void page::update_session(int id)
    {
        session_id_ = id;
        nxi::data::page::update(page_system_.user_database_, *this);
    }

    void page::update_mute(bool state)
    {
        is_muted_ = state;
        nxi::data::page::update(page_system_.user_database_, *this);
        emit event_update_mute(is_muted_);
    }

    void page::toggle_mute() { update_mute(!is_muted_); }

    void page::focus() { page_system_.focus(id_); }

    void page::load()
    {
        if (is_loaded_) return;
        is_loaded_ = true;
        nxi::data::page::set_loaded(page_system_.user_database_, id_, true);
        emit event_load();
    }

    void page::load(const QString& command)
    {
        update_command(command);
        emit event_load();
    }

    void page::reload() { emit event_reload(); }

    void page::close() { emit event_close(); }

    void page::run_script(const QString& script) const { nxi_warning("unimplemented"); }

    nxi::page_id page::id() const
    {
        // nxi_assert(ndb::is_valid(*this));
        return id_;
    }

    const QString& page::name() const { return name_; }
    const QString& page::command() const { return command_; }
    nxi::page_type page::type() const { return type_; }
    nxi::renderer_type page::renderer_type() const { return renderer_type_; }
    int page::session_id() const { return session_id_; }

    bool page::is_audible() const { return is_audible_; }
    bool page::is_loaded() const { return is_loaded_; }
    bool page::is_muted() const { return is_muted_; }

    bool page::has_color() const { return has_color_; }

    const QColor& page::color() const { return color_; }
    const QIcon& page::icon() const { return icon_; }
} // nxi