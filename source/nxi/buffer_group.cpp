#include <nxi/buffer_group.hpp>

namespace nxi
{
    buffer_group::buffer_group(int id)
        : id_{ id }
    {}

    int buffer_group::id() const { return id_; }
    nxi::suggestion_vector& buffer_group::suggestions() { return suggestions_; }
} // nxi