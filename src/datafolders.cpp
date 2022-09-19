#include "datafolders.hpp"

#include <QDir>
#include <QGuiApplication>

QFileInfo DataFolders::dataFolder()
{
    return QFileInfo{qGuiApp->applicationDirPath() + QDir::separator() + "data"};
}

QFileInfo DataFolders::dataImagesFolder()
{
    return QFileInfo{QDir{dataFolder().filePath()}.absoluteFilePath("images")};
}

QFileInfo DataFolders::dataProfilesFolder()
{
    return QFileInfo{QDir{dataFolder().filePath()}.absoluteFilePath("profiles")};
}
