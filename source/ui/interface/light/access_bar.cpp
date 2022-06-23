#include <ui/interface/light/access_bar.hpp>

#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <nxw/hbox_layout.hpp>

#include <ui/interface/light/button.hpp>
#include <ui/user.hpp>

#include <QLabel>

namespace ui::interfaces::light
{
    access_bar::access_bar(ui::user& user, ui::window*)
        : user_{ user }
    {
        connect(&user_.nxi_core(), &nxi::core::event_load, [this] {

        });

        auto layout = new nxw::hbox_layout;
        setLayout(layout);

        for (const auto& page : user_.nxi_user().page_system().pages())
        {
            auto btn = new button{ page->name(), this };
            btn->setFixedWidth(100);
            btn->setStyleSheet("background-color: transparent");
            btn->setMargin(4);
            layout->addWidget(btn);
            connect(btn, &button::event_enter, this, [btn] { btn->setStyleSheet("background-color: #DDDDDD"); });
            connect(btn, &button::event_leave, this, [btn] { btn->setStyleSheet("background-color: transparent"); });
        }

        layout->addStretch(1);
    }
} // nxi