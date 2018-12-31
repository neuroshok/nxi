#include <QtCore/QUrl>
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QApplication>
#include <QStyleHints>
#include <QScreen>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWebView/QtWebView>
#include <QQuickWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <nxw/icon_button.hpp>
#include <nxw/hbox_layout.hpp>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QtWebView::initialize();

    QQmlApplicationEngine engine;

    QWidget main;

    // navigation
    auto nav_layout = new nxw::hbox_layout;
    nav_layout->addWidget(new nxw::icon_button(&main, ":/button/history_previous"));
    nav_layout->addWidget(new nxw::icon_button(&main, ":/button/history_next"));
    nav_layout->addWidget(new nxw::icon_button(&main, ":/button/notification_none"));
    nav_layout->addWidget(new nxw::icon_button(&main, ":/button/download"));

    // widget container
    auto web_view = new QQuickWidget(&main);
    web_view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    web_view->setSource(QUrl("qrc:/android/web_view.qml"));

    auto main_layout = new QVBoxLayout;
    main_layout->addLayout(nav_layout);
    main_layout->addWidget(web_view);

    main.setLayout(main_layout);
    main.show();

    return app.exec();
}