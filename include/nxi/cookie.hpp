#ifndef INCLUDE_NXI_COOKIE_HPP_NXI
#define INCLUDE_NXI_COOKIE_HPP_NXI

#include <QWidget>

namespace nxi
{
    class cookie : public QNetworkCookie
    {
    public:
        void setSessionId(int id) { session_id_ = id; }

        int session_id() const { return session_id_; }

    private:
        int session_id_;
    };
} // nxi

#endif // INCLUDE_NXI_COOKIE_HPP_NXI