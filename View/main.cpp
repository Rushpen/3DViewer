#include <QApplication>
#include <QDebug>

#include "MainWindow.h"
#include "../database/settingsManager.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  if (!SettingsManager::connectToDatabase()) {
    qCritical() << "Ошибка при подключении к базе данных.";
    return -1;
  }

  s21::MainWindow mainWindow;
  mainWindow.setMinimumSize(400, 400);
  mainWindow.show();

  return app.exec();
}