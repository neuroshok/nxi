#include <platform/windows/notification.hpp>

#include <nxi/notification_data.hpp>

#include <nxi/log.hpp>
#include <wintoastlib.hpp>

using namespace WinToastLib;

namespace platform::windows
{
    class WinToastHandler : public WinToastLib::IWinToastHandler
    {
    public:
        WinToastHandler() {}
        // Public interfaces
        void toastActivated() const override {}
        void toastActivated(int actionIndex) const override
        {
            wchar_t buf[250];
            swprintf_s(buf, L"Button clicked: %d", actionIndex);
        }
        void toastDismissed(WinToastDismissalReason state) const override {}
        void toastFailed() const override {}

    private:
    };

    notification::notification(const nxi::notification_data& data)
        : handler_{ std::make_unique<WinToastHandler>() }
        , template_{ std::make_unique<WinToastLib::WinToastTemplate>() }
    {
        WinToast::instance()->setAppName(L"nxi");
        const auto aumi = WinToast::configureAUMI(L"neuroshok", L"nxi", L"nxi", L"20220805");
        WinToast::instance()->setAppUserModelId(aumi);
        if (!WinToast::instance()->initialize())
        {
            nxi_error("System not compatible");
        }

        *template_ = WinToastLib::WinToastTemplate(WinToastTemplate::Text04);

        auto first_line = "[" + nxi::notification_data::to_string<QString>(data.type) + "] " + data.source;
        template_->setTextField(first_line.toStdWString(), WinToastTemplate::FirstLine);
        template_->setTextField(data.title.toStdWString(), WinToastTemplate::SecondLine);
        template_->setTextField(data.message.toStdWString(), WinToastTemplate::ThirdLine);
    }

    void notification::show()
    {
        if (WinToast::instance()->showToast(*template_, handler_.get()) == -1L)
        {
            nxi_error("showToast error");
        }
    }
} // platform::windows