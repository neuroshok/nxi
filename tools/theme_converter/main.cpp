#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QJsonObject>

#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Missing nxi theme parameter";
        return 0;
    }

    std::string theme_nxi = *(argv + 1);
    std::cout << "args : " << theme_nxi;

    QJsonObject root;
    QJsonObject colors;
    colors["truc"] = 9;

    QJsonArray theme;
    theme.push_back(colors);
    theme.push_back(colors);

    root["theme"] = theme;

    QJsonDocument doc{ root };
    qDebug()<< doc.toJson();

    return 0;
}