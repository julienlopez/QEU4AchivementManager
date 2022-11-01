#include "achievementmodel.hpp"
#include "datamanager.hpp"
#include "profilemanager.hpp"

#include <QFileInfo>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    auto* data_manager = new DataManager(engine.networkAccessManager(), &app);
    auto* achievements_model = new AchievementModel(&app);
    QObject::connect(data_manager, &DataManager::achievementsChanged, achievements_model,
                     &AchievementModel::setAchievements);
    auto* profile_manager = new ProfileManager(&app);
    QObject::connect(data_manager, &DataManager::achievementsChanged, profile_manager,
                     &ProfileManager::setAchievements);

    engine.rootContext()->setContextProperty("image_directory", data_manager->dataImagesFolder().absoluteFilePath());
    engine.rootContext()->setContextProperty("data_manager", data_manager);
    engine.rootContext()->setContextProperty("achievements_model", achievements_model);
    engine.rootContext()->setContextProperty("profile_manager", profile_manager);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                     [url](QObject* obj, const QUrl& objUrl) {
                         if(!obj && url == objUrl) QCoreApplication::exit(-1);
                     },
                     Qt::QueuedConnection);
    engine.load(url);

    data_manager->checkOrDownloadData();

    return app.exec();
}
