#include <nxi/suggestion_vector.hpp>

namespace nxi
{

    dereference_iterator<suggestion_vector::suggestions_type::iterator> suggestion_vector::begin() { return suggestions_.begin(); }
    dereference_iterator<suggestion_vector::suggestions_type::iterator> suggestion_vector::end() { return suggestions_.end(); }
    dereference_iterator<suggestion_vector::suggestions_type::const_iterator> suggestion_vector::begin() const { return suggestions_.begin(); }
    dereference_iterator<suggestion_vector::suggestions_type::const_iterator> suggestion_vector::end() const { return suggestions_.end(); }

    void suggestion_vector::clear()
    {
        suggestions_.clear();
    }

    std::size_t suggestion_vector::size() const
    {
        return suggestions_.size();
    }

    const nxi::suggestion& suggestion_vector::operator[](unsigned int index) const
    {
        return *suggestions_[index];
    }
} // nxi