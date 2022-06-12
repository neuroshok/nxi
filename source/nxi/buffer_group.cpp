#include <nxi/buffer_group.hpp>

#include <nxi/core.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/buffer.hpp>

namespace nxi
{
    buffer_group::buffer_group(nxi::core& core, int id)
        : core_{ core }
        , id_{ id }
    {}

    //! load group using the default group data
    void buffer_group::load()
    {
        const auto& default_group = core_.user().buffer_system().group();
        set_command_root(default_group.command_root());
        set_page_root(default_group.page_root());
    }

    void buffer_group::exec()
    {
        if (!suggestions_.has_selection())
        {
            nxi_warning("nothing to execute");
            return;
        }

        suggestions_.selected().apply([this](auto&& suggestion) {
            using suggestion_type = std::decay_t<decltype(suggestion)>;
            if constexpr (std::is_same_v<suggestion_type, nds::node_ptr<const nxi::command>>)
            {
                core_.command_system().exec(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nds::node_ptr<nxi::page>>)
            {
                emit event_page_activate(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nxi::search_suggestion>)
            {
                core_.page_system().open<nxi::web_page>(suggestion.url() + suggestion.text());
                // core_.search(suggestion.text());
            }
            else nxi_warning("unknown suggestion");
        });
    }

    void buffer_group::set_command_root(nds::node_ptr<nxi::command> command)
    {
        command_root_ = command;
        emit event_command_root_update(command_root_);
    }
    void buffer_group::set_page_root(nds::node_ptr<nxi::page> page)
    {
        page_root_ = page;
        emit event_page_root_update(page_root_);
    }

    void buffer_group::context_suggest()
    {
        suggestions_.clear();
        core_.context_system().apply_on_active(
            [this](const nxi::contexts::command&) {
                core_.command_system().root_list([this](nds::node_ptr<const nxi::command> command) { suggestions_.push_back(command); });
            },
            [this](const nxi::contexts::page&) {
                for (auto& page : core_.page_system().root_targets())
                {
                    suggestions_.push_back(page);
                }
            },
            [this](const nxi::contexts::command_executor& ctx) { ctx.data.active_parameter().suggestion_callback(suggestions_); },
            [this](auto&&) { nxi_warning("no suggestion"); });
        emit event_action_update(suggestions_);
    }

    void buffer_group::suggest_command()
    {
        suggestions_.clear();
        core_.command_system().root_list([this](nds::node_ptr<const nxi::command> command) { suggestions_.push_back(command); });
        emit event_action_update(suggestions_);
    }

    void buffer_group::suggest_context()
    {
        suggestions_.clear();
        for (const auto& context : core_.context_system().contexts())
        {
            suggestions_.push_back(nxi::text_suggestion{ context->name(), "", QString::number(context->priority()) });
        };
        emit event_action_update(suggestions_);
    }

    void buffer_group::suggest_navigation()
    {
        suggestions_.clear();
        for (const auto& page_command : core_.navigation_system().page_command_logs())
        {
            suggestions_.push_back(page_command);
        }
        emit event_action_update(suggestions_);
    }

    void buffer_group::suggest_page()
    {
        suggestions_.clear();
        for (const auto& page : core_.page_system().root_targets())
        {
            suggestions_.push_back(page);
        }
        emit event_action_update(suggestions_);
    }

    void buffer_group::suggest_session()
    {
        suggestions_.clear();
        for (const auto& session : core_.session_system().sessions())
        {
            suggestions_.push_back(session->name());
        }
        emit event_action_update(suggestions_);
    }

    int buffer_group::id() const { return id_; }
    nds::node_ptr<nxi::command> buffer_group::command_root() const { return command_root_; }
    nds::node_ptr<nxi::page> buffer_group::page_root() const { return page_root_; }
    const nxi::suggestion_vector& buffer_group::suggestions() const { return suggestions_; }
    nxi::suggestion_vector& buffer_group::suggestions() { return suggestions_; }
} // nxi