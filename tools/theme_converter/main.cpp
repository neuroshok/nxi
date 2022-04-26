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


struct parametres
{
    int vers{};
    std::string manifest_vers{ "" };
    std::string path{ "" };
    std::string nom{ "" };
};

void generate(parametres param) 
{
    QJsonObject root;
    QJsonObject propriete;
    QJsonObject color;
    QJsonObject theme_frame;
    QJsonObject images;
    QJsonArray colors;
    QJsonArray theme;

    std::ifstream fichier_format_nxi{ param.path };

    if (!fichier_format_nxi.is_open())
    {
        std::cout << "Erreur : fichier non ouvert \n";
    }

     std::string ligne{ "" };

    while (std::getline(fichier_format_nxi, ligne))
    {
        auto espace_pos = ligne.find(" ");

        std::stringstream ss1, ss2, ss3, ss4;

        std::string prop{ ligne.begin(), ligne.begin() + espace_pos };

        std::string couleur_hexa_1{ ligne.begin() + espace_pos + 1, ligne.begin() + espace_pos + 3 };
        std::string couleur_hexa_2{ ligne.begin() + espace_pos + 3, ligne.begin() + espace_pos + 5 };
        std::string couleur_hexa_3{ ligne.begin() + espace_pos + 5, ligne.begin() + espace_pos + 7 };
        std::string couleur_hexa_4{ ligne.begin() + espace_pos + 7, ligne.begin() + espace_pos + 9 };

        std::cout << prop << "\n" << couleur_hexa_1 << "\n" << couleur_hexa_2 << "\n" << couleur_hexa_3 << "\n" << couleur_hexa_4 << "\n";
        unsigned int couleur_rgb_1;
        ss1 << std::hex << couleur_hexa_1;
        ss1 >> couleur_rgb_1;

        unsigned int couleur_rgb_2;
        ss2 << std::hex << couleur_hexa_2;
        ss2 >> couleur_rgb_2;

        unsigned int couleur_rgb_3;
        ss3 << std::hex << couleur_hexa_3;
        ss3 >> couleur_rgb_3;

        unsigned int couleur_rgb_4;
        std::ostringstream out;
        ss4 << std::hex << couleur_hexa_4;
        ss4 >> couleur_rgb_4;
        float couleur_rgb_4_adj = couleur_rgb_4 / 255.0f;
        out << std::setprecision(2) << couleur_rgb_4_adj;
        std::cout << out.str() << "\n";

        std::unordered_map<std::string, std::string> table_correspondance
        { 
            { "menu.background_color", "frame" },
            { "menu.text_color", "tab_background_text" } 
        };

        
        std::cout << "rgba(" + std::to_string(couleur_rgb_1) + "," + std::to_string(couleur_rgb_2) + "," + std::to_string(couleur_rgb_3) + "," + out.str() + ") \n";
        propriete[table_correspondance.at(prop).c_str()] = QString::fromStdString("rgba(" + std::to_string(couleur_rgb_1) + "," + std::to_string(couleur_rgb_2) + "," +
                                               std::to_string(couleur_rgb_3) + "," + out.str() + ") \n");
        colors.push_back(propriete);
    }

    theme_frame["theme_frame"] = "background.jpg";
    images["images"] = theme_frame;
    theme.push_back(images);

    color["colors"] = colors;
    theme.push_back(color);

    root["name"] = QString::fromStdString(param.nom);
    root["manifest_version"] = QString::fromStdString(param.manifest_vers);
    root["version"] = param.vers;
    root["theme"] = theme;

    QJsonDocument doc{ root };
    qDebug() << doc.toJson();
}


int main() //int main(int argc, char** argv)
{
   // if (argc < 2)
    //{
      //  std::cout << "Missing nxi theme parameter";
        //return 0;
    //}   
   // std::string theme_nxi = *(argv + 1);
   //std::cout << "args : "<< "test"; 
   // std::cout << "args : " << theme_nxi;

   parametres param{ 2, "1.0", "C:/test.txt", "nxi" };
    generate(param);

   
    
    return 0;
}
