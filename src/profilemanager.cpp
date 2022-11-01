#include "profilemanager.hpp"

#include "datafolders.hpp"

#include <QDebug>

ProfileManager::ProfileManager(QObject* parent)
    : QObject{parent}
    , m_current_profile{std::nullopt}
{
}

bool ProfileManager::hasACurrentProfile() const
{
    return m_current_profile.has_value();
}

QString ProfileManager::currentProfileName() const
{
    return m_current_profile.value_or("");
}

void ProfileManager::createNewProfile(const QString& name)
{
    qDebug() << "ProfileManager::createNewProfile(" << name << ")";
    m_current_profile = name;
    emit currentProfileChanged();
    emit currentProfileNameChanged(name);
}

void ProfileManager::loadProfile(const QString& name)
{
    m_current_profile = name;
}

int ProfileManager::availableProfiles() const
{
    return 0;
}
