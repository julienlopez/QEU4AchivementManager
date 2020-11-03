#include "achievementmodel.hpp"

#include <QFileInfo>

AchievementModel::AchievementModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int AchievementModel::rowCount(const QModelIndex& index) const
{
    return m_achievements.size();
}

QVariant AchievementModel::data(const QModelIndex& index, int role) const
{
    if(index.row() < 0 || index.row() >= rowCount()) return {};
    const auto& a = m_achievements[index.row()];
    switch(role)
    {
    case NameRole:
        return a.title;
    case ImageRole:
        return QFileInfo{a.image_url}.fileName();
    case DescriptionRole:
        return a.description;
    default:
        return {};
    }
}

QHash<int, QByteArray> AchievementModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ImageRole] = "image";
    roles[DescriptionRole] = "description";
    return roles;
}

void AchievementModel::setAchievements(QList<Achievement> achievements)
{
    beginResetModel();
    m_achievements = std::move(achievements);
    endResetModel();
}
