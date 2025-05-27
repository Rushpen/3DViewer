#include "ModelIO.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace s21 {
bool ModelIO::convertStlToObj(const QString& stlPath, QString& outObjPath) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(stlPath.toStdString(),
      aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

  if (!scene) return false;

  Assimp::Exporter exporter;
  QTemporaryFile tempFile(QDir::tempPath() + "/converted-XXXXXX.obj");
  tempFile.setAutoRemove(false);

  if (!tempFile.open()) return false;
  outObjPath = tempFile.fileName();
  tempFile.close();

  return exporter.Export(scene, "obj", outObjPath.toStdString()) == AI_SUCCESS;
}

bool ModelIO::exportModel(const QString& srcPath, const QString &format, QString& destPath) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(srcPath.toStdString(),
      aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

  if (!scene) return false;

  Assimp::Exporter exporter;
  std::string formatTo = format.toStdString();
  if(formatTo == "stl") { formatTo = "stlb";}
  return exporter.Export(scene, formatTo, destPath.toStdString()) == AI_SUCCESS;
}

}
