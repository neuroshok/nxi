#ifndef NXI_PAGE_WEB_H_NXI
#define NXI_PAGE_WEB_H_NXI

#include <nxi/page.hpp>

#include <functional>

class QWebEngineScript;

namespace nxi
{
    class page_system;

    class web_page : public nxi::page
    {
        Q_OBJECT
    public:
        web_page(nds::node_ptr<nxi::page>, nxi::page_system& ps);
        web_page(nds::node_ptr<nxi::page> ptr, nxi::page_system& ps, const QString& url);

        void add_script(const QWebEngineScript&) const;
        void run_script(const QString& source_code) const override;
        void run_script(const QString& source_code, std::function<void(const QVariant&)>) const;

    private:

        using const_qvariant = const QVariant; // nxi_bug_qt_moc

    signals:
        void event_add_script(const QWebEngineScript&) const;
        void event_call_script(const QString&, std::function<void(const_qvariant&)>) const;
        void event_run_script(const QString&) const;
        //void event_load_begin();
        //void event_load_end();
    };
} // nxi

#endif // NXI_PAGE_WEB_H_NXI
