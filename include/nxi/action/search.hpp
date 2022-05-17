//
// Created by Lezenn on 15/05/2022.
//

#ifndef INCLUDE_NXI_ACTION_SEARCH_HPP_NXI
#define INCLUDE_NXI_ACTION_SEARCH_HPP_NXI

#include <nxi/action.hpp>

namespace nxi
{
    /**
     * Search action
     *
     * Represents a search action from the command bar, using browser's default search engine.
     */
    class search_action : public nxi::action
    {
    public:
        search_action(QString text, QString name, QString url, QString icon = "") noexcept;

        [[nodiscard]] const QString& info() const noexcept override;
        [[nodiscard]] const QString& text() const noexcept override;
        [[nodiscard]] const QString& icon() const noexcept override;

        [[nodiscard]] virtual const QString& name() const noexcept;
        [[nodiscard]] virtual const QString& url() const noexcept;

    private:
        QString name_;
        QString info_;
        QString text_;
        QString icon_;
        QString url_;
    };
}

#endif // INCLUDE_NXI_ACTION_SEARCH_HPP_NXI
