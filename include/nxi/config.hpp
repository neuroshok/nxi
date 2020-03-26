#ifndef INCLUDE_NXI_CONFIG_HPP_NXI
#define INCLUDE_NXI_CONFIG_HPP_NXI

#include <ndb/persistent.hpp>

#include <nxi/preprocessor.hpp>

#include <string>

#define nxi_config_key3(NAME, TYPE, DEFAULT_VALUE) nxi::config_key<TYPE> NAME{ this, #NAME, DEFAULT_VALUE };
#define nxi_config_key4(NAME, TYPE, DEFAULT_VALUE, DESCRIPTION) nxi::config_key<TYPE> NAME{ this, #NAME, DEFAULT_VALUE, DESCRIPTION };

#define nxi_config_open(NAME) struct : ndb::persistent_group { using persistent_group::persistent_group;
#define nxi_config_close(GROUP_NAME) } GROUP_NAME{ this, #GROUP_NAME };
#define nxi_config_key(...) nxm_overload(nxi_config_key, __VA_ARGS__)

namespace nxi
{
    template<class T>
    class config_key : public ndb::persistent<T>
    {
    public:
        config_key(const ndb::persistent_group* group, std::string name, const T& default_value = T{}, std::string description = "")
            : ndb::persistent<T>(group, name, default_value)
            , description_{ std::move(description) }
        {}

        using ndb::persistent<T>::operator=;

        const std::string& description() const { return description_; }
    private:
        std::string description_;
    };

    struct config : ndb::persistent_group
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
    };
} // nxi

#endif // INCLUDE_NXI_CONFIG_HPP_NXI