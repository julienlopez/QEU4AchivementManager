#pragma once

#include "achievement.hpp"

#include <QAbstractListModel>

class AchievementModel : public QAbstractListModel
{
public:
    enum AchievementRoles
    {
        NameRole = Qt::UserRole + 1,
        ImageRole,
        DescriptionRole
    };

    explicit AchievementModel(QObject* parent = nullptr);

    virtual ~AchievementModel() = default;

    virtual int rowCount(const QModelIndex& index = {}) const override;

    virtual QVariant data(const QModelIndex& index, int role) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void setAchievements(QList<Achievement> achievements);

private:
    QList<Achievement> m_achievements;
};
