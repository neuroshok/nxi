#include <nxi/command/input.hpp>

#include <nxi/command.hpp>
#include <nxi/command/executor.hpp>
#include <nxi/context.hpp>
#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/command.hpp>
#include <nxi/user.hpp>

#include <QKeyEvent>

namespace nxi
{

    command_input::command_input(nxi::core& core)
        : core_{ core }
        , mode_{ mode_type::input }
        , state_{ state::search }
        , shortcut_input_{ *this }
    {
        /*
        connect(&core_.command_system(), &nxi::command_system::event_execution_request, [this](nxi::command_executor& executor)
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
                if (!input_.isEmpty() && !core_.context_system().is_active<nxi::contexts::command_executor>())
                {
                    suggestions_.push_back(nxi::search_suggestion{ input_, "Google", "https://www.google.com/search?q=", ":/icon/search" });
                    suggestions_.push_back(
                        nxi::search_suggestion{ input_, "CppReference", "https://en.cppreference.com/mwiki/index.php?search=", ":/icon/search" });
                }

                core_.context_system().apply_on_active(
                    [this](const nxi::contexts::command&) {
                        command_system().search(input_, [this](nds::node_ptr<const nxi::command> command) { suggestions_.push_back(command); });
                    },
                    [this](const nxi::contexts::page&) {
                        core_.page_system().search(input_, [this](nds::node_ptr<nxi::page> page) { suggestions_.push_back(page); });
                    },
                    [this](const nxi::contexts::command_executor& ctx) {
                        decltype(suggestions_) suggestions;
                        ctx.data.active_parameter().suggestion_callback(suggestions);
                        for (const auto& s : suggestions)
                        {
                            if (s.text().contains(input_)) suggestions_.push_back(s);
                        }
                    },
                    [this](auto&&) { nxi_warning("no suggestion"); });
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

    void command_input::set_mode(command_input::mode_type mode) { mode_ = mode; }

    command_input::mode_type command_input::mode() const { return mode_; }

    const QString& command_input::text() const { return input_; }

    bool command_input::is_empty() const { return input_.isEmpty(); }

    bool command_input::is_valid() const { return true; }

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

    const nxi::command_system& command_input::command_system() const { return core_.command_system(); }
    nxi::command_system& command_input::command_system() { return core_.command_system(); }

    nxi::shortcut_input& command_input::shortcut_input() { return shortcut_input_; }
} // nxi