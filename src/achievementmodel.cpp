#include "achievementmodel.hpp"

#include "icurrentprofilestatus.hpp"

#include <QFileInfo>

AchievementModel::AchievementModel(const ICurrentProfileStatus& current_profile_status, QObject* parent)
    : QAbstractListModel(parent)
    , m_current_profile_status(current_profile_status)
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
    case IsTodo:
        return m_current_profile_status.isTodo(a.title);
    case IsDone:
        return m_current_profile_status.isDone(a.title);
    default:
        return {};
    }
}

QHash<int, QByteArray> AchievementModel::roleNames() const
{
    return {{NameRole, "name"},
            {ImageRole, "image"},
            {DescriptionRole, "description"},
            {IsDone, "is_done"},
            {IsTodo, "is_todo"}};
}

void AchievementModel::setAchievements(QList<Achievement> achievements)
{
    beginResetModel();
    m_achievements = std::move(achievements);
    endResetModel();
}
