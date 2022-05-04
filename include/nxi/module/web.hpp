#ifndef NXI_MODULE_WEB_H_NXI
#define NXI_MODULE_WEB_H_NXI

#include <nxi/database.hpp>
#include <nxi/module.hpp>
#include <w3c/module/manifest.hpp>

#include <QHash>
#include <QWebEngineScript>

namespace nxi
{
    class user_session;

    class web_module : public module
    {
    public:
        struct background
        {
            std::vector<QString> scripts;
        };

        struct browser_action_data
        {
            bool browser_style = false;
            QString default_area;
            QString default_icon;
            QString default_popup;
            QString default_title;
        };

        struct commands
        {
            // std::map<QString, nxi::shortcut> shortcuts_;
        };

        struct content_script
        {
            enum class run_at_type { document_start, document_end, document_idle };
            bool all_frames;
            std::vector<QString> css;
            std::vector<QString> exclude_globs;
            std::vector<QString> exclude_matches;
            std::vector<QString> include_globs;
            std::vector<QString> js;
            bool match_about_blank;
            std::vector<QString> matches;
            run_at_type run_at;
        };

    public:
        web_module(nxi::user_session&, const QString& name);
        void init_scripts();

        void on_load() override;

        void process(nxi::web_page&) override;

        auto& browser_action() const { return browser_action_; }

    private:
        nxi::user_session& session_;

        w3c::manifest manifest_;
        QHash<QString, QWebEngineScript> scripts_;

        // applications
        QString author_;
        background background_;
        browser_action_data browser_action_;
        // chrome_settings_overrides
        // chrome_url_overrides
        commands commands_;
        std::vector<content_script> content_scripts_;
        // content_security_policy
        // default_locale
        QString description_;
        // developer
        // devtools_page
        QString homepage_url_;
        // icons
        // incognito
        int manifest_version_;
        QString name_;
        // omnibox
        // optional_permissions
        // option_ui
        // page_action
        // permissions
        // protocol_handlers
        QString short_name;
        // sidebar_action
        // theme
        QString version_;
        QString version_name_;
        // web_accessible_resources
    };
} // nxi

#endif // NXI_MODULE_WEB_H_NXI