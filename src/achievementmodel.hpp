#pragma once

#include "achievement.hpp"

#include <QAbstractListModel>

class ICurrentProfileStatus;

class AchievementModel : public QAbstractListModel
{
public:
    enum AchievementRoles
    {
        NameRole = Qt::UserRole + 1,
        ImageRole,
        DescriptionRole,
        IsTodo,
        IsDone
    };

    explicit AchievementModel(const ICurrentProfileStatus& current_profile_status, QObject* parent = nullptr);

    virtual ~AchievementModel() = default;

    virtual int rowCount(const QModelIndex& index = {}) const override;

    virtual QVariant data(const QModelIndex& index, int role) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void setAchievements(QList<Achievement> achievements);

private:
    const ICurrentProfileStatus& m_current_profile_status; 
    QList<Achievement> m_achievements;
};
