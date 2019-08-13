#include <QApplication>
#include <QMessageBox>

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <ui/core.hpp>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QMessageBox error;
    int status = 0;

    try
    {
        // init
        spdlog::set_level(spdlog::level::trace);

        nxi_trace("init nxi::core");
        nxi::core nxi_core;
        nxi_trace("init ui::core");
        ui::core ui_core(app, nxi_core);

        nxi_core.load();

        status = QApplication::exec();
    }
    catch (const std::exception& e)
    {
        error.setText(e.what());
        error.show();
        status = QApplication::exec();
    }

    return status;
}
