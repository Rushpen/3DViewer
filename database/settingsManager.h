#pragma once
#include <QColor>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace s21 {

struct OpenGLSettings {
  QColor backgroundColor;
  QColor vertexColor;
  int vertexSize;
  int vertexType;
  QColor edgeColor;
  int edgeWidth;
  bool isSolid;
};

struct ModelData {
  int id = -1;
  QString name;
  int verticesCount;
  int edgesCount;
};

struct RecentFileInfo {
  QString filename;
  QDateTime openedAt;
};

class SettingsManager {
 public:
  static bool connectToDatabase();

  static bool loadSettings(OpenGLSettings &settings, int modelId);
  static void saveSettings(const OpenGLSettings& settings, int modelId);

  static int saveOrFindModel(const ModelData& model);

  static bool insertRecentFile(const RecentFileInfo& recentInfo, const ModelData& model);
  static QVector<RecentFileInfo> loadRecentFiles();
};
}
