#include <QGuiApplication>
#include <QIcon>
#include <QSettings>
#include <QDebug>
#include <QQuickStyle>
#include <QQmlApplicationEngine>

#include <QFileSystemModel>

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("nxi");
    QGuiApplication::setOrganizationName("neuroshok");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    QSettings settings;
    QString style = QQuickStyle::name();
    if (!style.isEmpty())
        settings.setValue("style", style);
    else
        QQuickStyle::setStyle(settings.value("style").toString());

    QQmlApplicationEngine engine;
    //qDebug() << "zeta__________________ " <<  engine.importPathList();



    engine.load(QUrl("qrc:/main.qml"));

    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
