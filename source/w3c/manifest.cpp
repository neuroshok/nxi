#include <w3c/module//manifest.hpp>
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

            auto browser_action_obj = root["browser_action"].toObject();
            browser_action.browser_style = browser_action_obj["browser_style"].toBool();
            browser_action.default_area = browser_action_obj["default_area"].toString();
            browser_action.default_icon = browser_action_obj["default_icon"].toString();
            browser_action.default_popup = browser_action_obj["default_popup"].toString();
            browser_action.default_title = browser_action_obj["default_title"].toString();

            for (auto& item : root["content_scripts"].toArray())
            {
                content_scripts_t cs;
                auto content_script_item = item.toObject();
                for (auto& value : content_script_item["matches"].toArray()) cs.matches.push_back(value.toString());
                for (auto& value : content_script_item["js"].toArray()) cs.js.push_back(value.toString());
                content_scripts.push_back(std::move(cs));
            }
        }
    }
} // w3c

namespace nxi
{
    /*
    template<>
    std::vector<nxi::web_module::content_script> json_load_value<std::vector<nxi::web_module::content_script>>(const QJsonValue& values)
    {
        std::vector<nxi::web_module::content_script> vector;
        for (auto& obj : values.toArray())
        {
            std::vector<QString> matches;
            std::vector<QString> js;
            for (auto& value : obj.toObject()["matches"].toArray()) matches.push_back(value.toString());
            for (auto& value : obj.toObject()["js"].toArray()) js.push_back(value.toString());

            web_module::content_script cs;
            cs.matches = std::move(matches);
            cs.js = std::move(js);

            vector.emplace_back(std::move(cs));
        }
        return vector;
    }

     */
} // nxi
