//
// Created by Lezenn on 15/05/2022.
//

#ifndef INCLUDE_NXI_ACTION_GOTO_HPP_NXI
#define INCLUDE_NXI_ACTION_GOTO_HPP_NXI

#include <nxi/action.hpp>

namespace nxi
{

    /**
     * Goto action
     *
     * Loads a web page at the given URI
     */
    class goto_action : public nxi::action
    {
    public:
        explicit goto_action(QString uri, QString icon = "") noexcept;

        [[nodiscard]] const QString& info() const noexcept override;
        [[nodiscard]] const QString& text() const noexcept override;
        [[nodiscard]] const QString& icon() const noexcept override;

    private:
        QString text_;
        QString info_;
        QString icon_;
    };
}

#endif // INCLUDE_NXI_ACTION_GOTO_HPP_NXI
