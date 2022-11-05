#pragma once

#include "achievement.hpp"
#include "icurrentprofilestatus.hpp"
#include "profile.hpp"

#include <QObject>

#include <boost/optional.hpp>

class ProfileManager : public QObject, public ICurrentProfileStatus
{
    Q_OBJECT

    Q_PROPERTY(bool has_a_current_profile READ hasACurrentProfile NOTIFY currentProfileChanged)
    // Q_PROPERTY(int available_profile READ availableProfiles NOTIFY availableProfilesChanged)
    Q_PROPERTY(QString current_profile_name READ currentProfileName NOTIFY currentProfileNameChanged)

public:
    explicit ProfileManager(QObject* parent = nullptr);

    virtual ~ProfileManager() = default;

    bool hasACurrentProfile() const override;

    QString currentProfileName() const;

    Q_INVOKABLE void createNewProfile(const QString& name);

    Q_INVOKABLE void loadProfile(const QString& name);

    Q_INVOKABLE QStringList availableProfiles() const;

    bool isTodo(const QString& achievement_title) const override;

    bool isDone(const QString& achievement_title) const override;

public slots:
    void setAchievements(const QList<Achievement>& achievements);

signals:
    void currentProfileChanged();

    void currentProfileNameChanged(const QString&);

    void availableProfilesChanged(int);

private:
    boost::optional<Profile> m_current_profile;
    QList<Achievement> m_achievements;

    template <class Member> bool isProperty(const QString& achievement_title, Member mem_ptr) const;
};
