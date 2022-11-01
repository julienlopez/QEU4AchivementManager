#include "profilemanager.hpp"

#include "datafolders.hpp"
#include "utils.hpp"

#include <algorithm>

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
    std::transform(std::begin(m_achievements), std::end(m_achievements), std::back_inserter(res.achivements), &initAchievement);
    return res;
}

} // namespace

ProfileManager::ProfileManager(QObject* parent)
    : QObject{parent}
    , m_current_profile{boost::none}
{
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

void ProfileManager::setAchivements(const QList<Achievement>& achievements)
{
    m_achievements = achievements;
}
