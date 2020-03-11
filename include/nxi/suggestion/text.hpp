#ifndef INCLUDE_NXI_SUGGESTION_TEXT_HPP_NXI
#define INCLUDE_NXI_SUGGESTION_TEXT_HPP_NXI

#include <QString>

namespace nxi
{
    class text_suggestion
    {
    public:
        text_suggestion(QString text, QString icon = "", QString info = "");

        const QString& icon() const;
        const QString& info() const;
        const QString& text() const;

    private:
        QString icon_;
        QString info_;
        QString text_;
    };
} // nxi

#endif // INCLUDE_NXI_SUGGESTION_TEXT_HPP_NXI
