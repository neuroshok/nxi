#include <nxi/suggestion/vector.hpp>

#include <nxi/log.hpp>
#include <nxi/suggestion/text.hpp>

namespace nxi
{
    suggestion_vector::suggestions_type::iterator suggestion_vector::begin() { return suggestions_.begin(); }
    suggestion_vector::suggestions_type::iterator suggestion_vector::end() { return suggestions_.end(); }
    suggestion_vector::suggestions_type::const_iterator suggestion_vector::begin() const { return suggestions_.begin(); }
    suggestion_vector::suggestions_type::const_iterator suggestion_vector::end() const { return suggestions_.end(); }

    void suggestion_vector::push_back(QString text)
    {
        suggestions_.emplace_back(nxi::text_suggestion{ std::move(text) });
    }

    void suggestion_vector::push_back(const char* text)
    {
        push_back(QString{ text });
    }

    void suggestion_vector::select(int index) const
    {
        selected_index_ = index;
        emit event_selection_update(index);
    }

    void suggestion_vector::select_previous() const
    {
        if (selected_index_ > 0) select(selected_index_ - 1);
    }

    void suggestion_vector::select_next() const
    {
        if (selected_index_ + 1 < suggestions_.size()) select(selected_index_ + 1);
    }

    const suggestion_vector::suggestion_type& suggestion_vector::suggestion(int index) const
    {
        nxi_assert(index >= 0 && index < suggestions_.size());
        return suggestions_[index];
    }

    const suggestion_vector::suggestion_type& suggestion_vector::selected() const
    {
        return suggestion(selected_index_);
    }
    
    int suggestion_vector::selected_index() const
    {
        return selected_index_;
    }

    bool suggestion_vector::has_selection() const
    {
        return selected_index_ > -1;
    }

    void suggestion_vector::clear()
    {
        suggestions_.clear();
        selected_index_ = -1;
    }

    std::size_t suggestion_vector::size() const
    {
        return suggestions_.size();
    }

    const suggestion_vector::suggestion_type& suggestion_vector::operator[](unsigned int index) const
    {
        return suggestions_[index];
    }
} // nxi