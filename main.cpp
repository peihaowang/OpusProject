#include <time.h>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "MainWindow.h"
#include "ScribbleArea.h"

int main(int argc, char *argv[])
{
    std::srand(static_cast<uint>(std::time(nullptr)));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<ScribbleArea>("com.opus.components", 1, 0, "ScribbleArea");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

//    MainWindow w;
//#if defined (Q_OS_IOS)
//    w.showFullScreen();
//#else
//    w.showMaximized();
//#endif

    return app.exec();
}
