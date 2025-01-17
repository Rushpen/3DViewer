#ifndef UI_MENUBAR_H
#define UI_MENUBAR_H

#include <QAction>
#include <QColorDialog>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QWidgetAction>

#include "../3DViewer_back/mc/controller.h"
#include "../Record/Screen.h"
#include "UIMyDialog.h"

namespace s21 {

class MenuBarWidget : public QWidget {
  Q_OBJECT

 public:
  explicit MenuBarWidget(Controller *controller, QWidget *parent = nullptr);
  QDialog *getMyDialog() { return dialog; }
  QString getFilename() { return fileName; }
  QMenuBar *getMenuBar() { return menuBar; }
  QMenu *getEdgesMenu() { return EdgesMenu; }
  QMenu *getVertixesMenu() { return VertixesMenu; }

 signals:
  void fileLoaded(const std::vector<S21Matrix> &points,
                  const std::set<segment> &faces);
  void edgesColorChanged(const QColor &color);
  void vertixesColorChanged(const QColor &color);
  void backgroundColorChanged(const QColor &color);
  void vertixesSettingsChanged(const int &size, const int &typePoint);
  void edgesSettingsChanged(const int &width, const bool &isSolid);

 private:
  QMenuBar *menuBar;
  QMenu *MainMenu;
  QMenu *PreferencesMenu;
  QMenu *RecordMenu;

  QString fileName;

  QMenu *EdgesMenu;
  QAction *EdgesColorAction;
  QAction *EdgesTypeAction;

  QMenu *VertixesMenu;
  QAction *VertixesColorAction;
  QAction *VertixesTypeAction;

  QAction *exitAction;
  QAction *colorBackgroundAction;
  QAction *openFileAction;

  QWidgetAction *gifScreenshotAction;

  Controller *controller;
  MyDialog *dialog;
  GifScreenshotWidget *gifScreenshotWidget;

  void setupMenu();
  void setupConnections();
  void setupBackgroundConnection();
  void setupSettingsConnections();
  void setupEdgesConnections();
  void setupVertixesConnections();
  void openFileMenu();
};

}  // namespace s21

#endif