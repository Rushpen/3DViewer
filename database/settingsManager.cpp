#include "settingsmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

OpenGLSettings SettingsManager::loadSettings(const QString& path) {
  OpenGLSettings settings;
  QFile file(path);
  if (file.open(QIODevice::ReadOnly)) {
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();

    settings.backgroundColor = QColor(obj["background_color"].toString());
    settings.vertexColor = QColor(obj["vertix_Color"].toString());
    settings.vertexSize = obj["vertix_size"].toInt();
    settings.vertexType = obj["vertix_type"].toInt();
    settings.edgeColor = QColor(obj["edge_Color"].toString());
    settings.edgeWidth = obj["edge_width"].toInt();
    settings.isSolid = obj["edge_is_solid"].toBool();
    file.close();
  }
  return settings;
}

void SettingsManager::saveSettings(const QString& path, const OpenGLSettings& settings) {
  QJsonObject obj;
  obj["background_color"] = settings.backgroundColor.name();
  obj["vertix_Color"] = settings.vertexColor.name();
  obj["vertix_size"] = settings.vertexSize;
  obj["vertix_type"] = settings.vertexType;
  obj["edge_Color"] = settings.edgeColor.name();
  obj["edge_width"] = settings.edgeWidth;
  obj["edge_is_solid"] = settings.isSolid;

  QFile file(path);
  if (file.open(QIODevice::WriteOnly)) {
    file.write(QJsonDocument(obj).toJson());
    file.close();
  }
}

bool SettingsManager::insertRecentFile(const QString& path,
                                       const RecentFileInfo& recentInfo) {
  const QString jsonPath = path;
  QJsonArray recentArray;

  QFile file(jsonPath);
  if (file.open(QIODevice::ReadOnly)) {
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isArray())
      recentArray = doc.array();
    file.close();
  }

  for (int i = 0; i < recentArray.size(); ++i) {
    if (recentArray[i].toObject()["filename"].toString() == recentInfo.filename) {
      recentArray.removeAt(i);
      break;
    }
  }

  QJsonObject obj;
  obj["filename"] = recentInfo.filename;
  obj["model_name"] = recentInfo.modelName;
  obj["vertices_count"] = recentInfo.vertices;
  obj["edges_count"] = recentInfo.edges;
  obj["opened_at"] = recentInfo.openedAt.toString(Qt::ISODate);

  recentArray.prepend(obj);

  const int maxRecent = 10;
  while (recentArray.size() > maxRecent)
    recentArray.removeLast();

  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QJsonDocument saveDoc(recentArray);
    file.write(saveDoc.toJson());
    file.close();
    return true;
  }

  return false;
}

QVector<RecentFileInfo> SettingsManager::loadRecentFiles(const QString& path) {
  QVector<RecentFileInfo> result;

  QFile file(path);
  if (!file.open(QIODevice::ReadOnly))
    return result;

  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
  file.close();

  if (!doc.isArray())
    return result;

  for (const QJsonValue& val : doc.array()) {
    if (!val.isObject()) continue;

    QJsonObject obj = val.toObject();
    RecentFileInfo info;
    info.filename = obj["filename"].toString();
    info.modelName = obj["model_name"].toString();
    info.vertices = obj["vertices_count"].toInt();
    info.edges = obj["edges_count"].toInt();
    info.openedAt = QDateTime::fromString(obj["opened_at"].toString(), Qt::ISODate);
    result.append(info);
  }

  return result;
}
