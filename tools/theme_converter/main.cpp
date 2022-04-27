#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QJsonObject>

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <iomanip>


#define map_color(NXI_COLOR, W3C_COLOR){ #NXI_COLOR, #W3C_COLOR },
#define map_color_edit(NXI_COLOR, W3C_COLOR, IGNORED){#NXI_COLOR, #W3C_COLOR},
#define map_image(NXI_COLOR, W3C_COLOR)
std::unordered_map<std::string, std::string> table_correspondance
{ 
   #include <nxi/style/w3c_mapping.hpp>
};


struct parametres
{
    int manifest_vers{2};
    std::string path{ "" };
};


unsigned int hexa_to_rgb(std::string couleur_hexa)
{
    std::stringstream ss;
    unsigned int couleur_rgb;
    ss << std::hex << couleur_hexa;
    ss >> couleur_rgb;
    return couleur_rgb;
}


void generate(parametres param) 
{
    QJsonObject root;
    QJsonObject propriete;
    QJsonObject color;
    QJsonObject theme_frame;
    QJsonObject images;
    QJsonObject theme;
   

    std::ifstream fichier_format_nxi{ param.path };

    if (!fichier_format_nxi.is_open())
    {
        std::cout << "Erreur : fichier non ouvert \n";
        return;
    }

     std::string ligne{ "" };
     std::string nom;
     std::string vers;

    while (std::getline(fichier_format_nxi, ligne))
    {
        auto espace_pos = ligne.find(" ");
        if (espace_pos == std::string::npos)
        {
            continue;
        }

        std::string prop{ ligne.begin(), ligne.begin() + espace_pos };

        if (prop == "name")
        {
            
            nom = std::string {ligne.begin() + espace_pos + 1, ligne.end() };
          
        }

        else if (prop == "version")
        {
            vers = std::string{ ligne.begin() + espace_pos + 1, ligne.end() };
        }

        else
        {
     
            std::string couleur_hexa_1{ ligne.begin() + espace_pos + 1, ligne.begin() + espace_pos + 3 };
            std::string couleur_hexa_2{ ligne.begin() + espace_pos + 3, ligne.begin() + espace_pos + 5 };
            std::string couleur_hexa_3{ ligne.begin() + espace_pos + 5, ligne.begin() + espace_pos + 7 };
            std::string couleur_hexa_4{ ligne.begin() + espace_pos + 7, ligne.begin() + espace_pos + 9 };

            unsigned int couleur_rgb_1{ hexa_to_rgb(couleur_hexa_1) };
            unsigned int couleur_rgb_2{ hexa_to_rgb(couleur_hexa_2) };
            unsigned int couleur_rgb_3{ hexa_to_rgb(couleur_hexa_3) };
            unsigned int couleur_rgb_4{ hexa_to_rgb(couleur_hexa_4) };

            std::ostringstream out;
            float couleur_rgb_4_adj = couleur_rgb_4 / 255.0f;
            out << std::setprecision(2) << couleur_rgb_4_adj;

            std::string propriete_conv{ table_correspondance.at(prop).begin() + 7, table_correspondance.at(prop).end() };

            propriete[propriete_conv.c_str()] = QString::fromStdString("rgba(" + std::to_string(couleur_rgb_1) + "," + std::to_string(couleur_rgb_2) + "," + std::to_string(couleur_rgb_3) + "," + out.str() + ")");
        }
    }

   theme_frame["theme_frame"] = "background.jpg";

    root["name"] = QString::fromStdString(nom);
    root["version"] = QString::fromStdString(vers);
    root["manifest_version"] = param.manifest_vers;
    theme["images"] = theme_frame;
    theme["colors"] = propriete;
    root["theme"]=theme;
    

    QJsonDocument doc{ root };
    //qDebug() << doc.toJson();
    std::ofstream fichier{ "manifest.json" };
    fichier << doc.toJson().toStdString();
}


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Missing nxi theme parameter";
        return 0;
    }
    std::string theme_nxi = *(argv + 1);
    std::cout << "args : " << "test \n"; //std::cout << "args : " << theme_nxi;

    parametres param;
    param.path = "C:/test.txt"; // param.path = theme_nxi;
    param.manifest_vers = 2;
    generate(param);

    return 0;
}
