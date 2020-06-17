#include <nxi/system/session.hpp>

#include <nxi/database.hpp>
#include <nxi/database/queries/session.hpp>
#include <nxi/log.hpp>

namespace nxi
{
    session_system::session_system(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
    {}

    void session_system::load()
    {
        nxi_trace("");

        // load active session
        ndb::result result = nxi::queries::get_active_sessions();
        //if (result)

    }

} // nxi
