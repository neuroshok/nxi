#include <w3c/module/manifest.hpp>
#include <nxi/log.hpp>

#include <vector>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

namespace w3c
{
    manifest::manifest(const QString& name)
    {
        load(name);
    }

    void manifest::load(const QString& module_name)
    {
        QFile module_file("./module/webextension/"+ module_name +"/manifest.json");

        if (!module_file.open(QFile::ReadOnly)) nxi_warning("module manifest missing : {}", module_file.fileName());
        else
        {
            QJsonDocument doc = QJsonDocument::fromJson(module_file.readAll());
            module_file.close();
            auto root = doc.object();
            name = root["name"].toString();

            if (!root["background"].isNull())
            {
                auto background_obj = root["background"].toObject();
                background.page = background_obj["page"].toString();
                background.persistent = background_obj["persistent"].toBool();
                for (const auto& item : background_obj["scripts"].toArray()) background.scripts.push_back(item.toString());
            }

            if (!root["browser_action"].isNull())
            {
                auto browser_action_obj = root["browser_action"].toObject();
                browser_action.browser_style = browser_action_obj["browser_style"].toBool();
                browser_action.default_area = browser_action_obj["default_area"].toString();
                browser_action.default_icon = browser_action_obj["default_icon"].toString();
                browser_action.default_popup = browser_action_obj["default_popup"].toString();
                browser_action.default_title = browser_action_obj["default_title"].toString();
                for (const auto& item : browser_action_obj["theme_icons"].toArray())
                {
                    auto theme_icons_obj = item.toObject();
                    browser_action.theme_icons.push_back(
                        browser_action_t::theme_icons_t{
                            theme_icons_obj["light"].toString()
                            , theme_icons_obj["light"].toString()
                            , theme_icons_obj["size"].toInt()}
                        );
                }
            }

            for (const auto& item : root["content_scripts"].toArray())
            {
                content_scripts_t cs;
                auto content_script_item = item.toObject();
                for (const auto& value : content_script_item["matches"].toArray()) cs.matches.push_back(value.toString());
                for (const auto& value : content_script_item["js"].toArray()) cs.js.push_back(value.toString());
                content_scripts.push_back(std::move(cs));
            }
        }
    }
} // w3c