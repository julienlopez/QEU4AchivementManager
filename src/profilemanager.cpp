#include "profilemanager.hpp"

#include "datafolders.hpp"
#include "filedownloader.hpp"
#include "utils.hpp"

#include <algorithm>

#include <QDir>
#include <QDirIterator>
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

QDir profilesDirectory()
{
    return DataFolders::dataProfilesFolder().absoluteFilePath();
}

void writeProfile(const Profile& profile)
{
    const auto file_path = profilesDirectory().absoluteFilePath(profile.name + ".json");
    FileDownloader::writeFile(file_path, QJsonDocument{toJson(profile)}.toJson());
}

auto hasTitle(const QString& achievement_title)
{
    return [&achievement_title](const AchievementStatus& ach) { return ach.achievement_title == achievement_title; };
}

QString removeExtension(const QString& str)
{
    const auto pos = str.lastIndexOf('.');
    if(pos < str.size()) return str.left(str.size() - pos - 1);
    return str;
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
    qDebug() << "ProfileManager::loadProfile(" << name << ")";
    // TODO ProfileManager::loadProfile
    // m_current_profile = name;
}

QStringList ProfileManager::availableProfiles() const
{
    auto res = profilesDirectory().entryList(QDir::Files | QDir::NoDotAndDotDot);
    std::transform(std::begin(res), std::end(res), std::begin(res), &removeExtension);
    return res;
}

bool ProfileManager::isTodo(const QString& achievement_title) const
{
    return isProperty(achievement_title, &AchievementStatus::is_todo);
}

bool ProfileManager::isDone(const QString& achievement_title) const
{
    return isProperty(achievement_title, &AchievementStatus::is_done);
}

void ProfileManager::setAchievements(const QList<Achievement>& achievements)
{
    m_achievements = achievements;
}

template <class Member> bool ProfileManager::isProperty(const QString& achievement_title, Member mem_ptr) const
{
    if(!hasACurrentProfile()) return false;
    const auto it = std::find_if(std::begin(m_current_profile->achivements), std::end(m_current_profile->achivements),
                                 hasTitle(achievement_title));
    if(it == std::end(m_current_profile->achivements)) return false;
    return (*it).*mem_ptr;
}