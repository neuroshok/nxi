#include <nxi/suggestion.hpp>

namespace nxi
{
    const QString& suggestion::icon() const { return apply([this](auto&& n) -> decltype(auto) { return icon(n); }); }
    const QString& suggestion::info() const { return apply([this](auto&& n) -> decltype(auto) { return info(n); }); }
    const QString& suggestion::text() const { return apply([this](auto&& n) -> decltype(auto) { return text(n); }); }


    //const QString& suggestion::icon(nds::node_ptr<const nxi::command> data) const { return data->icon(); }
} // nxi