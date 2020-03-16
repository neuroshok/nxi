#include <nxi/command/input.hpp>

#include <nxi/command.hpp>
#include <nxi/command/executor.hpp>
#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/system/command.hpp>

#include <QKeyEvent>

namespace nxi
{

    command_input::command_input(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
        , command_system_{ nxi_core_.command_system() }
        , mode_{ mode::input }
        , state_{ state::search }
        , shortcut_input_{ *this }
    {
        /*
        connect(&nxi_core_.command_system(), &nxi::command_system::event_execution_request, [this](nxi::command_executor& executor)
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
        if (mode_ == mode::shortcut && event->isAutoRepeat()) return;

        if (event->type() == QEvent::KeyPress)
        {
            if (mode_ != mode::shortcut && shortcut_input_.is_trigger_key(key)) set_mode(mode::shortcut);

            if (input_.isEmpty() && mode_ == mode::input)
            {
                //reset();
                return;
            }
            suggestions_.clear();

            if (mode_ == mode::shortcut)
            {
                shortcut_input_.update(event, suggestions_);
                emit event_shortcut_input_update(shortcut_input_.to_string());
            }
            if (mode_ == mode::input)
            {
                // add user input as first suggestion
                if (!input_.isEmpty())
                {
                    suggestions_.push_back(nxi::search_suggestion{ input_, "Google", "https://www.google.com/search?q=", ":/icon/search" } );
                    suggestions_.push_back(nxi::search_suggestion{ input_, "CppReference", "https://en.cppreference.com/mwiki/index.php?search=", ":/icon/search" } );
                }

                command_system_.search(input_, [this](nds::node_ptr<const nxi::command> command)
                {
                    suggestions_.push_back(std::move(command));
                });
            }

            if (suggestions_.size() > 0) suggestions_.select(0);
            emit event_suggestion_update(suggestions_);
        }

        // shortcut mode need release event
        if (mode_ == mode::shortcut && event->type() == QEvent::KeyRelease)
        {
            shortcut_input_.update(event, suggestions_);
            if (!shortcut_input_.is_triggered()) set_mode(mode::input);
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
                nxi_core_.command_system().exec(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nds::node_ptr<const nxi::page>>)
            {
                //nxi_core_.page_system().load(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nxi::search_suggestion>)
            {
                nxi_core_.page_system().open<nxi::web_page>(0, suggestion.url() + suggestion.text());
                //nxi_core_.search(suggestion.text());
            }
            else nxi_warning("unknown suggestion");
        });
    }

    void command_input::set_mode(command_input::mode mode)
    {
        mode_ = mode;
    }

    const nxi::suggestion_vector& command_input::suggestions() const
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

    void command_input::suggest_command()
    {
        suggestions_.clear();
        command_system_.root_list([this](nds::node_ptr<const nxi::command> command)
        {
            suggestions_.push_back(std::move(command));
        });
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_page()
    {
        suggestions_.clear();
        for (const auto& page :  nxi_core_.page_system().list_root())
        {
            suggestions_.push_back(page);
        }
        emit event_suggestion_update(suggestions_);
    }

    void command_input::reset()
    {
        set_mode(mode::input);
        clear();
        emit event_reset();
    }

    void command_input::clear()
    {
        input_ = "";
        suggestions_.clear();
    }

    const nxi::command_system& command_input::command_system() const
    {
        return command_system_;
    }

    nxi::shortcut_input& command_input::shortcut_input()
    {
        return shortcut_input_;
    }
} // nxi