#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "controller.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    Controller ctl;
    
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    auto ctx = engine.rootContext();
    ctx->setContextProperty("Controller", &ctl);
    engine.load(url);
    
    return app.exec();
}
