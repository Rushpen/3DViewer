#pragma once
#include <QColor>
#include <QString>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QVector>

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
  static OpenGLSettings loadSettings(const QString& path);
  static void saveSettings(const QString& path, const OpenGLSettings& settings);
  static bool insertRecentFile(const QString& path,
                                       const RecentFileInfo& recentInfo);
  static QVector<RecentFileInfo> loadRecentFiles(const QString& path);
};
