#include <nxi/style.hpp>

#include <QString>
#include <QFile>

#include <w3c/theme.hpp>
#include <include/nxi/style.hpp>
#include <qdebug.h>

#include <ui/command.hpp>
#include <ui/interface/main.hpp>
#include <ui/interface/control_bar.hpp>
#include <include/w3c/theme.hpp>
#include <include/nxi/log.hpp>

#include <QBrush>
#include <include/nxw/icon_button.hpp>
#include <include/nxi/style_data.hpp>

namespace nxi
{
    style::style() : name_{ "nxi" } {}
    style::style(const QString& name) : name_{ name } {}

    void style::load()
    {
        QFile module_file("./module/theme/"+ name_ + "/manifest.json");
        if (!module_file.open(QFile::ReadOnly)) qDebug() << "read error : " << module_file.fileName();
        else
        {
            nxi_trace("log theme {}", name_);
            QJsonDocument doc = QJsonDocument::fromJson(module_file.readAll());
            QJsonObject obj = doc.object();
            module_file.close();


            w3c::theme w3c_theme{ obj["theme"] };
            from_w3c(w3c_theme);
        }
    }

    void style::update(ui::interfaces::main* ui) const
    {
        //ui->style.background_image = data_.image;
        //ui->control_bar_ = data_.control_bar.command.height
        //ui->setPalette(data_.palette);
    }

    void style::update(ui::command* ui) const
    {
        /*
        ui->setFrame(false);
        ui->setMinimumHeight(data_.control_bar.command.height);
        ui->setPalette(data_.control_bar.command.palette);
        auto font = ui->font();
        font.setBold(true);
        ui->setFont(font);*/
    }

    void style::update(ui::interfaces::control_bar* ui) const
    {
        for (nxw::icon_button* w : ui->findChildren<nxw::icon_button*>())
        {
            // update(w);
            //w->setFixedSize(data_.control_bar.icon_button.size, 32);
            w->setFixedSize(32, 32);
            w->style.icon_color = data_.icon_button.icon_color.get();

        }
    }

    //void style::paint(nxw::icon_button*)    {    }


    void style::update(QWidget* ui) const
    {

    }

    void style::update(ui::views::page_tree* ui) const
    {
        //ui->setPalette(data_.page_bar.palette);
    }

    #define map_color(W3C_KEY, NXI_PALETTE, GROUP, ROLE) if (!W3C_KEY.is_null()) NXI_PALETTE.setColor(GROUP, ROLE, W3C_KEY.get().toQColor());
    void style::from_w3c(w3c::theme& theme)
    {
        auto& nx = data_;
        using group = QPalette::ColorGroup;
        using role = QPalette::ColorRole;

        /*
        nx.image = "./module/theme/"+ name_ + "/" + theme.images.theme_frame.get();

        map_color(theme.colors.frame, nx.palette, group::All, role::Base);

        map_color(theme.colors.popup_text, nx.palette, group::All, role::Text);

        map_color(theme.colors.tab_background_text, nx.page_bar.palette, group::All, role::Text);

        map_color(theme.colors.bookmark_text, nx.page_bar.palette, group::Current, role::Text);
        map_color(theme.colors.toolbar_text, nx.page_bar.palette, group::Current, role::Text);
        map_color(theme.colors.toolbar_text, nx.page_bar.palette, group::Current, role::Highlight);

        map_color(theme.colors.bookmark_text, nx.control_bar.palette, group::All, role::Text);

        map_color(theme.colors.toolbar_field, nx.control_bar.command.palette, group::All, role::Base);
        map_color(theme.colors.toolbar_field_text, nx.control_bar.command.palette, group::All, role::Text);*/
    }
    #undef map_color
} // nxi