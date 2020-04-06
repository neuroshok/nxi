#ifndef INCLUDE_W3C_MODULE_MANIFEST_HPP_NXI
#define INCLUDE_W3C_MODULE_MANIFEST_HPP_NXI

//! https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/manifest.json

#include <unordered_map>
#include <variant>
#include <vector>

#include <QString>

namespace w3c
{
    struct manifest
    {
        QString author;
        QString name;

        struct background_t
        {
            QString page;
            bool persistent;
            std::vector<QString> scripts;
        } background;

        struct browser_action_t
        {
            struct theme_icons_t
            {
                QString dark;
                QString light;
                int size;
            };

            bool browser_style = false;
            QString default_area;
            std::variant<QString, std::unordered_map<int, QString>> default_icon;
            QString default_popup;
            QString default_title;
            std::vector<theme_icons_t> theme_icons;
        } browser_action;

        // ignore browser_specific_settings

        struct chrome_settings_overrides_t
        {
            struct search_provider_t
            {
                QString name;
                QString search_url;
                bool is_default;
                std::vector<QString> alternate_urls;
                QString encoding;
                QString favicon_url;
                QString image_url;
                QString image_url_post_params;
                QString instant_url;
                QString instant_url_post_params;
                QString keyword;
                int prepopulated_id;
                QString search_url_post_params;
                QString suggest_url;
                QString suggest_url_post_params;
            };
            QString homepage;
        } chrome_settings_overrides;

        struct chrome_url_overrides_t
        {
            QString bookmarks;
            QString history;
            QString newtab;
        } chrome_url_overrides;

        struct command_t
        {
            QString suggested_key; // use single shortcut, ignore os specific shortcut from w3c
            QString description;
            // special actions
            QString execute_browser_action;
            QString execute_page_action;
            QString execute_sidebar_action;
        };
        std::vector<std::pair<QString, command_t>> commands;

        struct content_scripts_t
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
        std::vector<content_scripts_t> content_scripts;

        QString content_security_policy;
        QString default_locale;
        QString description;

        struct developer_t
        {
            QString name;
            QString url;
        };

        QString devtools_page;

        // dictionaries // what is this ?

        struct externally_connectable_t
        {
            std::vector<QString> ids;
            std::vector<QString> matches;
        } externally_connectable;

        QString homepage_url;
        std::unordered_map<int, QString> icons;
        enum class incognito{ not_allowed, spanning, split };
        int manifest_version;
        QString name;

        struct omnibox_t
        {
            QString keyword;
        } omnibox;

        std::vector<QString> optional_permissions;

        struct options_ui_t
        {
            bool browser_style;
            bool open_in_tab;
            QString page;
        } options_ui;

        struct page_action_t
        {
            struct theme_icons_t
            {
                QString dark;
                QString light;
                int size;
            };

            bool browser_style = false;
            std::variant<QString, std::unordered_map<int, QString>> default_icon;
            QString default_popup;
            QString default_title;
            std::vector<QString> hide_matches;
            std::vector<QString> show_matches;
            bool pinned = true;
        } page_action;

        std::vector<QString> permissions;

        struct protocol_handlers_t
        {
            QString protocol;
            QString name;
            QString uriTemplate;
        } protocol_handlers;

        QString short_name;

        // sidebar_action // unsupported in nxi ui

        struct storage_t
        {
            QString managed_schema;
        } storage;

        //! themes are not part of modules in nxi
        // theme
        // theme_experiment

        struct user_scripts_t
        {
            QString api_script;
        } user_scripts;

        QString version;
        QString version_name;
        std::vector<QString> web_accessible_resources;

        manifest(const QString& name);
        void load(const QString& name);
    };
} // w3c

#endif // INCLUDE_W3C_MODULE_MANIFEST_HPP_NXI