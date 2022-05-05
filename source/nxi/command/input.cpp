#include <nxi/command/input.hpp>

#include <nxi/command.hpp>
#include <nxi/command/executor.hpp>
#include <nxi/context.hpp>
#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/command.hpp>
#include <nxi/user_session.hpp>

#include <QKeyEvent>

namespace nxi
{

    command_input::command_input(nxi::user_session& session)
        : session_{ session }
        , mode_{ mode_type::input }
        , state_{ state::search }
        , shortcut_input_{ *this }
    {
        /*
        connect(&session_.command_system(), &nxi::command_system::event_execution_request, [this](nxi::command_executor& executor)
        {
            state_ = state::executing;
            //command_input().reset();
            command_executor_.emplace( command );
            setText("");
            setPlaceholderText("enter parameter " + command_executor_->active_parameter().name);
        });*/

    }

    void command_input::update(const QString& input, QKeyEvent* event)
    {
        input_ = input;
        auto key = static_cast<Qt::Key>(event->key());

        // ignore autorepeat in shortcut mode
        if (mode_ == mode_type::shortcut && event->isAutoRepeat()) return;

        if (event->type() == QEvent::KeyPress)
        {
            if (mode_ != mode_type::shortcut && shortcut_input_.is_trigger_key(key)) set_mode(mode_type::shortcut);

            if (input_.isEmpty() && mode_ == mode_type::input)
            {
                reset();
                return;
            }
            suggestions_.clear();

            if (mode_ == mode_type::shortcut)
            {
                shortcut_input_.update(event, suggestions_);
                emit event_shortcut_input_update(shortcut_input_.to_string());
            }
            if (mode_ == mode_type::input)
            {
                if (!input_.isEmpty() && !session_.context_system().is_active<nxi::contexts::command_executor>())
                {
                    suggestions_.push_back(nxi::search_suggestion{ input_, "Google", "https://www.google.com/search?q=", ":/icon/search" } );
                    suggestions_.push_back(nxi::search_suggestion{ input_, "CppReference", "https://en.cppreference.com/mwiki/index.php?search=", ":/icon/search" } );
                }

                session_.context_system().apply_on_active
                (
                    [this](const nxi::contexts::command&)
                    {
                        command_system().search(input_, [this](nds::node_ptr<const nxi::command> command)
                        {
                            suggestions_.push_back(std::move(command));
                        });
                    }
                    , [this](const nxi::contexts::page&)
                    {
                        session_.page_system().search(input_, [this](nds::node_ptr<nxi::page> page)
                        {
                            suggestions_.push_back(std::move(page));
                        });
                    }
                    , [this](const nxi::contexts::command_executor& ctx)
                    {
                        decltype(suggestions_) suggestions;
                        ctx.data.active_parameter().suggestion_callback(suggestions);
                        for (const auto& s : suggestions) { if (s.text().contains(input_)) suggestions_.push_back(s); }
                    }
                    , [this](auto&&) { nxi_warning("no suggestion"); }
                );
            }

            if (suggestions_.size() > 0) suggestions_.select(0);
            emit event_suggestion_update(suggestions_);
        }

        // shortcut mode need release event
        if (mode_ == mode_type::shortcut && event->type() == QEvent::KeyRelease)
        {
            shortcut_input_.update(event, suggestions_);
            if (!shortcut_input_.is_triggered()) set_mode(mode_type::input);
            emit event_shortcut_input_update(shortcut_input_.to_string());
            emit event_suggestion_update(suggestions_);
        }
    }

    void command_input::exec()
    {
        if (!suggestions_.has_selection())
        {
            nxi_warning("nothing to execute");
            return;
        }
        nxi_trace(" exec : {} ", input_);

        suggestions_.selected().apply([this](auto&& suggestion)
        {
            using suggestion_type = std::decay_t<decltype(suggestion)>;
            if constexpr (std::is_same_v<suggestion_type, nds::node_ptr<const nxi::command>>)
            {
                session_.command_system().exec(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nds::node_ptr<nxi::page>>)
            {
                session_.page_system().focus(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nxi::search_suggestion>)
            {
                session_.page_system().open<nxi::web_page>(suggestion.url() + suggestion.text());
                //session_.search(suggestion.text());
            }
            else nxi_warning("unknown suggestion");
        });
    }

    void command_input::set_mode(command_input::mode_type mode)
    {
        mode_ = mode;
    }

    command_input::mode_type command_input::mode() const { return mode_; }

    const nxi::suggestion_vector& command_input::suggestions() const
    {
        return suggestions_;
    }

    nxi::suggestion_vector& command_input::suggestions()
    {
        return suggestions_;
    }

    const QString& command_input::text() const
    {
        return input_;
    }

    bool command_input::is_empty() const
    {
        return input_.isEmpty();
    }

    bool command_input::is_valid() const
    {
        return true;
    }

    void command_input::context_suggest()
    {
        suggestions_.clear();
        session_.context_system().apply_on_active
        (
            [this](const nxi::contexts::command&)
            {
                command_system().root_list([this](nds::node_ptr<const nxi::command> command)
                {
                    suggestions_.push_back(std::move(command));
                });
            }
            , [this](const nxi::contexts::page&)
            {
                for (auto& page :  session_.page_system().root_targets())
                {
                    suggestions_.push_back(page);
                }
            }
            , [this](const nxi::contexts::command_executor& ctx) { ctx.data.active_parameter().suggestion_callback(suggestions_); }
            , [this](auto&&) { nxi_warning("no suggestion"); }
        );
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_command()
    {
        suggestions_.clear();
        command_system().root_list([this](nds::node_ptr<const nxi::command> command)
        {
            suggestions_.push_back(std::move(command));
        });
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_context()
    {
        suggestions_.clear();
        for (const auto& context : session_.context_system().contexts())
        {
            suggestions_.push_back(nxi::text_suggestion{ context->name(), "", QString::number(context->priority()) });
        };
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_navigation()
    {
        suggestions_.clear();
        for (const auto& page_command :  session_.navigation_system().page_command_logs())
        {
            suggestions_.push_back(page_command);
        }
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_page()
    {
        suggestions_.clear();
        for (const auto& page :  session_.page_system().root_targets())
        {
            suggestions_.push_back(page);
        }
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_session()
    {
        suggestions_.clear();
        for (const auto& session :  session_.session_system().sessions())
        {
            suggestions_.push_back(session->name());
        }
        emit event_suggestion_update(suggestions_);
    }

    void command_input::reset()
    {
        set_mode(mode_type::input);
        clear();
        emit event_reset();
    }

    void command_input::clear()
    {
        input_ = "";
        emit event_input_update(input_);
        suggestions_.clear();
    }

    const nxi::command_system& command_input::command_system() const
    {
        return session_.command_system();
    }
    nxi::command_system& command_input::command_system()
    {
        return session_.command_system();
    }

    nxi::shortcut_input& command_input::shortcut_input()
    {
        return shortcut_input_;
    }
} // nxi