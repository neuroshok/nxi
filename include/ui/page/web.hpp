#ifndef UI_PAGE_WEB_H_NXI
#define UI_PAGE_WEB_H_NXI

#include <ui/page.hpp>

namespace nxi { class web_page; }

class QWebEnginePage;

namespace ui
{
    class user_session;
    class web_engine_page;

    class web_page : public ui::page
    {
        Q_OBJECT
    public:
        web_page(ui::user_session&, nxi::web_page& page);

        void load(const QString& url);
        QWebEnginePage* native();

    private:
        void display(renderer*) override;
        ui::renderer* make_renderer() const override;

    private:
        ui::user_session& session_;
        nxi::web_page& page_;

        ui::web_engine_page* native_page_;
    };

} // ui

#endif // UI_PAGE_WEB_H_NXI