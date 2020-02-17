#include <nxi/command/shortcut_input.hpp>

#include <nxi/system/command.hpp>

#include <sstream>

/*
 * !NEW
 * CTRL ALT + S
 * CTRL + W S
 *
 * search
 */

namespace nxi
{
    shortcut_input::shortcut_input()
    {
        untrigger();
    }

    nxi::commands_view shortcut_input::update(nxi::command_system& command_system, QKeyEvent* event)
    {
        if (event->isAutoRepeat()) return suggestions_;

        Qt::Key key = static_cast<Qt::Key>(event->key());

        if (event->type() == QEvent::KeyRelease)
        {
            combo_keys_.erase( std::remove( combo_keys_.begin(), combo_keys_.end(), key ), combo_keys_.end() );

            if (trigger_mode_ == trigger_mode::combo) search(command_system, event);
        }

        if (event->type() == QEvent::KeyPress)
        {
            search(command_system, event);
        }

        /*
        qDebug() << "___COMBO";
        for (auto k : combo_keys_) qDebug() << k;

        qDebug() << "___SEQ";
        for (auto k : sequence_keys_) qDebug() << k;
*/

        return suggestions_;
    }

    void shortcut_input::untrigger()
    {
        combo_keys_.clear();
        sequence_keys_.clear();
        input_.clear();
        executed_ = false;
        trigger_mode_ = trigger_mode::combo;
    }

    void shortcut_input::search(nxi::command_system& command_system, QKeyEvent* event)
    {
        Qt::Key key = static_cast<Qt::Key>(event->key());

        suggestions_.clear();

        bool combo_match = false;
        bool sequence_match = false;

        command_system.for_each([this, &key, &combo_match, &sequence_match](auto&& node)
        {
            const nxi::shortcut& shortcut = node->get().shortcut();

            // shortcut has combo keys
            if (trigger_mode_ == trigger_mode::combo && shortcut.combo_keys.size() > 0)
            {
                // check previous input combo keys without the new key
                bool combo_partial_match = std::equal(combo_keys_.begin(), combo_keys_.end(), shortcut.combo_keys.begin());

                if (combo_partial_match)
                {
                    auto next = combo_keys_.size();

                    // combo + key match
                    if (next < shortcut.combo_keys.size() && key == shortcut.combo_keys[next])
                    {
                        qDebug() << "combo + key match : " << shortcut.to_string();
                        suggestions_.push_back(stz::observer_ptr(&node->get()));
                        combo_match = true;
                    }
                }
            }

            // shortcut has sequence keys
            if (shortcut.sequence_keys.size() > 0)
            {
                // check previous input combo keys without the new key
                bool sequence_partial_match = std::equal(sequence_keys_.begin(), sequence_keys_.end(), shortcut.sequence_keys.begin());

                qDebug() << "sequence_partial_match  : " << sequence_partial_match;

                if (sequence_partial_match)
                {
                    auto next = sequence_keys_.size();

                    if (next < shortcut.sequence_keys.size()) qDebug() << "CHECK  : " << key << " " << shortcut.sequence_keys[next];
                    // sequence + key match
                    if (next < shortcut.sequence_keys.size() && key == shortcut.sequence_keys[next])
                    {
                        qDebug() << "sequence_keys + key match : " << shortcut.to_string();
                        suggestions_.push_back(stz::observer_ptr(&node->get()));
                        sequence_match = true;

                        if (sequence_keys_.size() + 1 == shortcut.sequence_keys.size())
                        {
                            qDebug() << "FULLMATCH" << shortcut.to_string();
                        }
                    }
                }
            }
        }); // for_each

        qDebug() << "Result : " << combo_match << " " << sequence_match;
        // combo has priority
        if (combo_match) sequence_match = false;

        // switch mode if no combo match
        if (sequence_match) trigger_mode_ = trigger_mode::sequence;

        if (trigger_mode_ == trigger_mode::combo) combo_keys_.push_back(key);
        if (trigger_mode_ == trigger_mode::sequence) sequence_keys_.push_back(key);



        // no match reset input
        if (!combo_match && ! sequence_match) untrigger();
    }

} // nxi