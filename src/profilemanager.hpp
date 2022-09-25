#pragma once

#include <QObject>

#include <optional>

class ProfileManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool has_a_current_profile READ hasACurrentProfile NOTIFY currentProfileChanged)
    Q_PROPERTY(int available_profile READ availableProfiles NOTIFY availableProfilesChanged)
    Q_PROPERTY(QString current_profile_name READ currentProfileName NOTIFY currentProfileNameChanged)

public:
    explicit ProfileManager(QObject* parent = nullptr);

    virtual ~ProfileManager() = default;

    bool hasACurrentProfile() const;

    QString currentProfileName() const;

    Q_INVOKABLE void createNewProfile(const QString& name);

    void loadProfile(const QString& name);

    int availableProfiles() const;

signals:
    void currentProfileChanged();

    void currentProfileNameChanged(const QString&);

    void availableProfilesChanged(int);

private:
    std::optional<QString> m_current_profile;
};
