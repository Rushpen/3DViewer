#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  s21::MainWindow mainWindow;
  mainWindow.setMinimumSize(400, 400);
  mainWindow.show();

  return app.exec();
}