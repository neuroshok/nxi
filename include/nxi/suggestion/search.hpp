#ifndef INCLUDE_NXI_SUGGESTION_SEARCH_HPP_NXI
#define INCLUDE_NXI_SUGGESTION_SEARCH_HPP_NXI

#include <QString>

namespace nxi
{
    class search_suggestion
    {
    public:
        search_suggestion(QString text, QString name, QString url, QString icon = "");

        const QString& name() const;
        const QString& icon() const;
        const QString& info() const;
        const QString& text() const;
        const QString& url() const;

    private:
        QString name_;
        QString icon_;
        QString info_;
        QString text_;
        QString url_;
    };
} // nxi

#endif // INCLUDE_NXI_SUGGESTION_SEARCH_HPP_NXI
