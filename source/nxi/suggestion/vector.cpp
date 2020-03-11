#include <nxi/suggestion/vector.hpp>

#include <nxi/suggestion/text.hpp>

namespace nxi
{
    suggestion_vector::suggestions_type::iterator suggestion_vector::begin() { return suggestions_.begin(); }
    suggestion_vector::suggestions_type::iterator suggestion_vector::end() { return suggestions_.end(); }
    suggestion_vector::suggestions_type::const_iterator suggestion_vector::begin() const { return suggestions_.begin(); }
    suggestion_vector::suggestions_type::const_iterator suggestion_vector::end() const { return suggestions_.end(); }

    void suggestion_vector::add(QString text)
    {
        suggestions_.emplace_back(nxi::text_suggestion{ std::move(text) });
    }

    void suggestion_vector::add(const char* text)
    {
        add(QString{ text });
    }

    void suggestion_vector::clear()
    {
        suggestions_.clear();
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