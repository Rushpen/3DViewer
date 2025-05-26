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

  float cameraRadius = 1.0f;
  float cameraAngleX = 0.0f;
  float cameraAngleY = 0.0f;

  std::vector<float> orthoParams = { -1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f };
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

  static QString serializeOrthoParams(const std::vector<float>& params);
  static std::vector<float> deserializeOrthoParams(const QString& str);

  static bool loadSettings(OpenGLSettings &settings, int modelId);
  static void saveSettings(const OpenGLSettings& settings, int modelId);

  static int saveOrFindModel(const ModelData& model);

  static bool insertRecentFile(const RecentFileInfo& recentInfo, const ModelData& model);
  static QVector<RecentFileInfo> loadRecentFiles();
};
}
