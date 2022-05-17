//
// Created by Lezenn on 15/05/2022.
//

#ifndef INCLUDE_NXI_ACTION_TEXT_HPP_NXI
#define INCLUDE_NXI_ACTION_TEXT_HPP_NXI

#include <nxi/action.hpp>

namespace nxi
{

    /**
     * Text action
     *
     * Shows a simple text
     */
    class text_action : public nxi::action
    {
    public:
        text_action(QString text, QString info, QString icon = "") noexcept;

        [[nodiscard]] const QString& info() const noexcept override;
        [[nodiscard]] const QString& text() const noexcept override;
        [[nodiscard]] const QString& icon() const noexcept override;

    private:
        QString text_;
        QString info_;
        QString icon_;
    };
}

#endif // INCLUDE_NXI_ACTION_TEXT_HPP_NXI
