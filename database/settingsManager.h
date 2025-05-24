#pragma once
#include <QColor>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

struct OpenGLSettings {
  QColor backgroundColor;
  QColor vertexColor;
  int vertexSize;
  int vertexType;
  QColor edgeColor;
  int edgeWidth;
  bool isSolid;
};

struct RecentFileInfo {
  QString filename;
  QString modelName;
  int vertices;
  int edges;
  QDateTime openedAt;
};

class SettingsManager {
 public:
  static bool connectToDatabase();
  static bool loadSettings(OpenGLSettings &settings);
  static void saveSettings(const OpenGLSettings& settings);
  static bool insertRecentFile(const RecentFileInfo& recentInfo);
  static QVector<RecentFileInfo> loadRecentFiles();
};
