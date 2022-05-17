//
// Created by Lezenn on 15/05/2022.
//

#ifndef INCLUDE_NXI_ACTION_COMMAND_HPP_NXI
#define INCLUDE_NXI_ACTION_COMMAND_HPP_NXI

#include <nds/graph/node.hpp>
#include <nxi/action.hpp>

namespace nxi
{
    class command;

    /**
     * Command action
     */
    class command_action : public nxi::action
    {
    public:
        explicit command_action(nds::node_ptr<const nxi::command> data) noexcept;

        [[nodiscard]] const QString& info() const noexcept override;
        [[nodiscard]] const QString& text() const noexcept override;
        [[nodiscard]] const QString& icon() const noexcept override;

    private:
        nds::node_ptr<const nxi::command> data_;
    };
}

#endif // INCLUDE_NXI_ACTION_COMMAND_HPP_NXI
