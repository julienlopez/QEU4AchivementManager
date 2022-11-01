#include "profilemanager.hpp"

#include "datafolders.hpp"
#include "filedownloader.hpp"
#include "utils.hpp"

#include <algorithm>

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

namespace
{

AchievementStatus initAchievement(const Achievement& achievements)
{
    return {achievements.title, false, false};
}

Profile newProfile(const QString& name, const QList<Achievement>& m_achievements)
{
    // return {name, m_achievements | std::ranges::transform_view(&initAchievement)};
    Profile res{name, {}};
    std::transform(std::begin(m_achievements), std::end(m_achievements), std::back_inserter(res.achivements),
                   &initAchievement);
    return res;
}

QJsonObject achivementToJson(const AchievementStatus& achievement)
{
    QJsonObject res;
    res["achievement_title"] = achievement.achievement_title;
    res["is_done"] = achievement.is_done;
    res["is_todo"] = achievement.is_todo;
    return res;
}

QJsonArray toJson(const QList<AchievementStatus>& achievements)
{
    QJsonArray res;
    std::transform(std::begin(achievements), std::end(achievements), std::back_inserter(res), &achivementToJson);
    return res;
}

QJsonObject toJson(const Profile& profile)
{
    QJsonObject res;
    res["name"] = profile.name;
    res["achivements"] = toJson(profile.achivements);
    return res;
}

void writeProfile(const Profile& profile)
{
    const QDir directory{DataFolders::dataProfilesFolder().absoluteFilePath()};
    const auto file_path = directory.absoluteFilePath(profile.name + ".json");
    FileDownloader::writeFile(file_path, QJsonDocument{toJson(profile)}.toJson());
}

} // namespace

ProfileManager::ProfileManager(QObject* parent)
    : QObject{parent}
    , m_current_profile{boost::none}
{
    if(!DataFolders::dataFolder().isDir()) QDir().mkdir(DataFolders::dataFolder().filePath());
    if(!DataFolders::dataProfilesFolder().isDir()) QDir().mkdir(DataFolders::dataProfilesFolder().filePath());
}

bool ProfileManager::hasACurrentProfile() const
{
    return m_current_profile.has_value();
}

QString ProfileManager::currentProfileName() const
{
    return m_current_profile.map(Utils::mem_var(&Profile::name)).value_or("");
}

void ProfileManager::createNewProfile(const QString& name)
{
    qDebug() << "ProfileManager::createNewProfile(" << name << ")";
    m_current_profile = newProfile(name, m_achievements);
    writeProfile(*m_current_profile);
    emit currentProfileChanged();
    emit currentProfileNameChanged(name);
}

void ProfileManager::loadProfile(const QString& name)
{
    // TODO ProfileManager::loadProfile
    // m_current_profile = name;
}

int ProfileManager::availableProfiles() const
{
    return 0;
}

void ProfileManager::setAchievements(const QList<Achievement>& achievements)
{
    m_achievements = achievements;
}
