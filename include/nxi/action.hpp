//
// Created by Lezenn on 15/05/2022.
//

#ifndef INCLUDE_NXI_ACTION_HPP_NXI
#define INCLUDE_NXI_ACTION_HPP_NXI

#include <QString>

namespace nxi
{
    /**
     * Action representation interface
     *
     * Shown in action suggestion of the command bar
     */
    class action
    {
    public:
        /**
         * Action description
         * @return description
         */
        [[nodiscard]] virtual const QString& info() const noexcept = 0;

        /**
         * Value used by the action
         * @return Value used
         */
        [[nodiscard]] virtual const QString& text() const noexcept = 0;

        /**
         * Icon used in the command bar suggestions
         * @return Icon's path
         */
        [[nodiscard]] virtual const QString& icon() const noexcept = 0;
    };
}

#endif // INCLUDE_NXI_ACTION_HPP_NXI
