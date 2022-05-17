//
// Created by Lezenn on 15/05/2022.
//

#ifndef INCLUDE_NXI_ACTION_PAGE_HPP_NXI
#define INCLUDE_NXI_ACTION_PAGE_HPP_NXI

#include <nds/graph/node.hpp>
#include <nxi/action.hpp>

namespace nxi
{
    class page;

    class page_action : public nxi::action
    {
    public:
        explicit page_action(nds::node_ptr<nxi::page> page) noexcept;

        [[nodiscard]] const QString& info() const noexcept override;
        [[nodiscard]] const QString& text() const noexcept override;
        [[nodiscard]] const QString& icon() const noexcept override;

    private:
        nds::node_ptr<nxi::page> page_;
    };
} // nxi

#endif // INCLUDE_NXI_ACTION_PAGE_HPP_NXI
