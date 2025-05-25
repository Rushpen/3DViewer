#include "settingsmanager.h"

namespace s21 {

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
    CREATE TABLE IF NOT EXISTS model (
    id SERIAL PRIMARY KEY,
    name TEXT NOT NULL,
    vertixesCount INTEGER,
    edgesCount INTEGER
  )
  )");

  bool ok2 =  query.exec(R"(
    CREATE TABLE IF NOT EXISTS scene_settings (
    id SERIAL PRIMARY KEY,
    background_color TEXT,
    vertex_color TEXT,
    vertex_size INTEGER,
    vertex_type INTEGER,
    edge_color TEXT,
    edge_width INTEGER,
    is_solid BOOLEAN,
    model_id INTEGER NOT NULL UNIQUE REFERENCES model(id) ON DELETE CASCADE
  )
  )");

  bool ok3 = query.exec(R"(
    CREATE TABLE IF NOT EXISTS recent_files (
      id SERIAL PRIMARY KEY,
      filename TEXT NOT NULL,
      model_id INTEGER NOT NULL REFERENCES model(id) ON DELETE CASCADE,
      opened_at TIMESTAMP
    )
  )");
  
  return ok1 && ok2 && ok3;
}

void SettingsManager::saveSettings(const OpenGLSettings& settings, int modelId) {
    QSqlDatabase db = QSqlDatabase::database(); 
    if (!db.isOpen()) return;

    QSqlQuery query(db);

    query.prepare(R"(
        INSERT INTO scene_settings (
            background_color, vertex_color, vertex_size, vertex_type, 
            edge_color, edge_width, is_solid, model_id
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT (model_id) DO UPDATE SET 
        background_color = EXCLUDED.background_color,
        vertex_color = EXCLUDED.vertex_color,
        vertex_size = EXCLUDED.vertex_size,
        vertex_type = EXCLUDED.vertex_type,
        edge_color = EXCLUDED.edge_color,
        edge_width = EXCLUDED.edge_width,
        is_solid = EXCLUDED.is_solid
    )");
    
    query.addBindValue(settings.backgroundColor.name());
    query.addBindValue(settings.vertexColor.name());
    query.addBindValue(settings.vertexSize);
    query.addBindValue(settings.vertexType);
    query.addBindValue(settings.edgeColor.name());
    query.addBindValue(settings.edgeWidth);
    query.addBindValue(settings.isSolid);
    query.addBindValue(modelId);

    if (!query.exec()) {
        qDebug() << "Error inserting settings:" << query.lastError().text();
        return;
    }
}

bool SettingsManager::loadSettings(OpenGLSettings &settings, int modelId) {
  QSqlDatabase db = QSqlDatabase::database();
  QSqlQuery query(db);

  query.prepare(R"(
    SELECT background_color, vertex_color, vertex_size, vertex_type,
           edge_color, edge_width, is_solid
    FROM scene_settings
    WHERE model_id = ?
    LIMIT 1
  )");

  query.addBindValue(modelId);
  if (!query.exec()) {
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

int SettingsManager::saveOrFindModel(const ModelData& model) {
  QSqlDatabase db = QSqlDatabase::database(); 
  if (!db.isOpen()) return false;

  QSqlQuery query(db);

  query.prepare(R"(
    SELECT id FROM model
    WHERE name = :name AND vertixesCount = :vertices AND edgesCount = :edges
  )");
  query.bindValue(":name", model.name);
  query.bindValue(":vertices", model.verticesCount);
  query.bindValue(":edges", model.edgesCount);

  if (query.exec() && query.next()) {
    return query.value(0).toInt();
  }

  query.prepare(R"(
    INSERT INTO model (name, vertixesCount, edgesCount)
    VALUES (?, ?, ?) RETURNING id
  )");

  query.addBindValue(model.name);
  query.addBindValue(model.verticesCount);
  query.addBindValue(model.edgesCount);

  if (!query.exec() || !query.next()) {
    qDebug() << "Error inserting model:" << query.lastError().text();
    return -1;
  }

  return query.value(0).toInt();
}


bool SettingsManager::insertRecentFile(const RecentFileInfo& recentInfo, const ModelData& model) {
  QSqlDatabase db = QSqlDatabase::database(); 
  if (!db.isOpen()) return false;

  const int modelId = saveOrFindModel(model);
  if (modelId < 0) return false;

  QSqlQuery query(db);

  query.prepare("DELETE FROM recent_files WHERE filename = :filename");
  query.bindValue(":filename", recentInfo.filename);

  if (!query.exec()) {
    qDebug() << "Error deleting existing file:" << query.lastError().text();
    return false;
  }

  query.prepare(R"(
    INSERT INTO recent_files (filename, model_id, opened_at)
    VALUES (:filename, :model_id, :opened_at)
  )");
  query.bindValue(":filename", recentInfo.filename);
  query.bindValue(":model_id", modelId);
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
  query.prepare(R"(
                SELECT filename, model_id, opened_at 
                FROM recent_files
                ORDER BY opened_at DESC 
                LIMIT 10
                )");
    if (!query.exec()) {
      qDebug() << "Error loading recent files:" << query.lastError().text();
      return result;
    }

  while (query.next()) {
    RecentFileInfo info;
    info.filename = query.value("filename").toString();
    info.openedAt = query.value("opened_at").toDateTime();
    result.append(info);
  }

  return result;
}
}

