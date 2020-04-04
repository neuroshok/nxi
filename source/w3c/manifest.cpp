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

            for (auto& obj : root["content_scripts"].toArray())
            {
                content_scripts_t cs;
                for (auto& value : obj.toObject()["matches"].toArray()) cs.matches.push_back(value.toString());
                for (auto& value : obj.toObject()["js"].toArray()) cs.js.push_back(value.toString());
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
