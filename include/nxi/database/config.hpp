#ifndef DATABASE_CONFIG_H_NXI
#define DATABASE_CONFIG_H_NXI

#include <ndb/persistent.hpp>

#include <string>

namespace nxi
{
    struct config : ndb::persistent_group
    {
        using persistent_group::persistent_group;

        struct : ndb::persistent_group
        {
            using persistent_group::persistent_group;

            ndb::persistent<std::string> home{ this, "home", "nxi.neuroshok.com" };
            ndb::persistent<std::string> download_path{ this, "download_path", "d:/incoming" };

            struct : ndb::persistent_group
            {
                using persistent_group::persistent_group;

                ndb::persistent<int> preview_size{ this, "preview_size", 10 };
                ndb::persistent<float> suggest_ratio{ this, "download_path", 0.2f };

            } history{ this, "history" };

        } browser{ this, "browser" };
    };
} // nxi

#endif // DATABASE_CONFIG_H_NXI