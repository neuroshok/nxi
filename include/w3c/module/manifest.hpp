#ifndef INCLUDE_W3C_MODULE_MANIFEST_HPP_NXI
#define INCLUDE_W3C_MODULE_MANIFEST_HPP_NXI

//! https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/manifest.json

#include <vector>

#include <QString>

namespace w3c
{
    struct manifest
    {
        QString name;

        struct browser_action_t
        {
            bool browser_style = false;
            QString default_area;
            QString default_icon;
            QString default_popup;
            QString default_title;
        } browser_action;

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

        manifest(const QString& name);
        void load(const QString& name);
    };
} // w3c

#endif // INCLUDE_W3C_MODULE_MANIFEST_HPP_NXI