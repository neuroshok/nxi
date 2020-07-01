#ifndef INCLUDE_NXI_CONFIG_HPP_NXI
#define INCLUDE_NXI_CONFIG_HPP_NXI

#include <string>
#include <QString>

namespace nxi
{
    struct config
    {
        struct
        {
            QString home;
        } browser;
    };

    /*    struct config : // ndb::persistent_group
    {
        using persistent_group::persistent_group;

        nxi_config_open(command)
            nxi_config_key(autoexec, bool, false, "exec command on single match");
        nxi_config_close(command)

        nxi_config_open(page)
            nxi_config_key(root, int, 0, "Page root");
        nxi_config_close(page)

        nxi_config_open(browser)
            nxi_config_key(home, std::string, "www.google.com");
            nxi_config_key(download_path, std::string, "d:/incoming");

            nxi_config_key(page_unload_timeout, int, 20, "Duration before unloading a closed page");
            nxi_config_key(page_mute_mode, int, 0, "0: Normal\n1: Auto mute non-active pages");
            nxi_config_key(page_fullscreen_mode, int, 0, "0: Fullscreen\n1: Fullwindow\n2: Hybrid");

            nxi_config_open(history)
                nxi_config_key(preview_size, int, 10);
                nxi_config_key(suggest_ratio, double, 0.2);
            nxi_config_close(history)

            nxi_config_open(interface)
                nxi_config_key(show_page_bar, bool, true);
            nxi_config_close(interface)

        nxi_config_close(browser)
    };*/
} // nxi

#endif // INCLUDE_NXI_CONFIG_HPP_NXI