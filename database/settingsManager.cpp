#include "settingsmanager.h"

bool SettingsManager::connectToDatabase() {
  QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
  db.setHostName("localhost");
  db.setDatabaseName("viewer_db");
  db.setUserName("postgres");
  db.setPassword("1");

  if (!db.open()) {
    qDebug() << "Failed to connect to DB:" << db.lastError().text();
    return false;
  }

  QSqlQuery query(db);
  bool ok1 = query.exec(R"(
    CREATE TABLE IF NOT EXISTS recent_files (
      id SERIAL PRIMARY KEY,
      filename TEXT NOT NULL,
      model_name TEXT,
      vertices INTEGER,
      edges INTEGER,
      opened_at TIMESTAMP
    )
  )");

  bool ok2 =  query.exec(R"(
    CREATE TABLE IF NOT EXISTS scene_settings (
    background_color TEXT,
    vertex_color TEXT,
    vertex_size INTEGER,
    vertex_type INTEGER,
    edge_color TEXT,
    edge_width INTEGER,
    is_solid BOOLEAN
  )
  )");
  return ok1 && ok2;
}

void SettingsManager::saveSettings(const OpenGLSettings& settings) {
    QSqlDatabase db = QSqlDatabase::database(); 
    if (!db.isOpen()) return;

    QSqlQuery query(db);

    if (!query.exec("DELETE FROM scene_settings")) {
        qDebug() << "Error deleting existing settings:" << query.lastError().text();
        return;
    }

    query.prepare(R"(
        INSERT INTO scene_settings (
            background_color, vertex_color, vertex_size, vertex_type, 
            edge_color, edge_width, is_solid
        ) VALUES (?, ?, ?, ?, ?, ?, ?)
    )");
    
    query.addBindValue(settings.backgroundColor.name());
    query.addBindValue(settings.vertexColor.name());
    query.addBindValue(settings.vertexSize);
    query.addBindValue(settings.vertexType);
    query.addBindValue(settings.edgeColor.name());
    query.addBindValue(settings.edgeWidth);
    query.addBindValue(settings.isSolid);

    if (!query.exec()) {
        qDebug() << "Error inserting settings:" << query.lastError().text();
        return;
    }
}

bool SettingsManager::loadSettings(OpenGLSettings &settings) {
  QSqlDatabase db = QSqlDatabase::database();
  QSqlQuery query(db);

  if (!query.exec(R"(SELECT
                  background_color, vertex_color, vertex_size, vertex_type, edge_color, edge_width, is_solid 
                  FROM scene_settings
                  LIMIT 1)")) {
    qDebug() << "Failed to load settings from DB:" << query.lastError().text();
    return false;
  }

  if (!query.next()) { return false; }

  settings.backgroundColor = QColor(query.value(0).toString());
  settings.vertexColor = QColor(query.value(1).toString());
  settings.vertexSize = query.value(2).toInt();
  settings.vertexType = query.value(3).toInt();
  settings.edgeColor = QColor(query.value(4).toString());
  settings.edgeWidth = query.value(5).toInt();
  settings.isSolid = query.value(6).toBool();

  return true;
}


bool SettingsManager::insertRecentFile(const RecentFileInfo& recentInfo) {
  QSqlDatabase db = QSqlDatabase::database(); 
  if (!db.isOpen()) return false;

  QSqlQuery query(db);

  query.prepare("DELETE FROM recent_files WHERE filename = :filename");
  query.bindValue(":filename", recentInfo.filename);

  if (!query.exec()) {
    qDebug() << "Error deleting existing file:" << query.lastError().text();
    return false;
  }

  query.prepare(R"(
    INSERT INTO recent_files (filename, model_name, vertices, edges, opened_at)
    VALUES (:filename, :model_name, :vertices, :edges, :opened_at)
  )");
  query.bindValue(":filename", recentInfo.filename);
  query.bindValue(":model_name", recentInfo.modelName);
  query.bindValue(":vertices", recentInfo.vertices);
  query.bindValue(":edges", recentInfo.edges);
  query.bindValue(":opened_at", recentInfo.openedAt);

  if (!query.exec()) {
    qDebug() << "Error inserting recent file:" << query.lastError().text();
    return false;
  }

  return true;
}

QVector<RecentFileInfo> SettingsManager::loadRecentFiles() {
  QVector<RecentFileInfo> result;
  QSqlDatabase db = QSqlDatabase::database();
  if (!db.isOpen()) return result;

  QSqlQuery query(db);
  if (!query.exec("SELECT filename, model_name, vertices, edges, opened_at "
                  "FROM recent_files ORDER BY opened_at DESC LIMIT 10")) {
    qDebug() << "Error loading recent files:" << query.lastError().text();
    return result;
  }

  while (query.next()) {
    RecentFileInfo info;
    info.filename = query.value("filename").toString();
    info.modelName = query.value("model_name").toString();
    info.vertices = query.value("vertices").toInt();
    info.edges = query.value("edges").toInt();
    info.openedAt = query.value("opened_at").toDateTime();
    result.append(info);
  }

  return result;
}

