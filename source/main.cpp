#include <QApplication>
#include <QMessageBox>
#include <QtWebEngine>

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <ui/core.hpp>

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

    QtWebEngine::initialize();

    QApplication app(argc, argv);
    QMessageBox error;
    int status = 0;

    try
    {
        // init
        init_logger();

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
