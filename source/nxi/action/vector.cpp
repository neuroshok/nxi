//
// Created by Lezenn on 15/05/2022.
//

#include <nxi/action/text.hpp>
#include <nxi/action/vector.hpp>

namespace nxi
{
    action_vector::actions_type::iterator action_vector::begin() { return actions_.begin(); }

    action_vector::actions_type::iterator action_vector::end() { return actions_.end(); }

    action_vector::actions_type::const_iterator action_vector::begin() const { return actions_.begin(); }

    action_vector::actions_type::const_iterator action_vector::end() const { return actions_.end(); }

    void action_vector::push_back(QString text) { actions_.emplace_back(nxi::text_action{ std::move(text), "" }); }

    void action_vector::push_back(const char* text) { push_back(QString{ text }); }

    void action_vector::erase(int index)
    {
        nxi_assert(index >= 0 && index < actions_.size());
        actions_.erase(actions_.begin() + index);
        emit event_update(stz::make_observer(this));
    }

    void action_vector::erase(nxi::page_action action)
    {
        auto it = std::find_if(actions_.begin(), actions_.end(), [&action](auto& s) {
            return s.apply([&action](nxi::page_action page) { return page == action; }

            );
        });
    }

} // nxi