#ifndef UI_MENUBAR_H
#define UI_MENUBAR_H

#include <QAction>
#include <QColorDialog>
#include <QDebug>
#include <QVector>
#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QWidgetAction>
#include <QSettings>
#include <QProcess>

#include "../3DViewer_back/mc/controller.h"
#include "../Record/Screen.h"
#include "../../database/settingsManager.h"
#include "../3DViewer_back/ModelIO/ModelIO.h"
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
  GifScreenshotWidget *getGifScreenshotWidget() const { return gifScreenshotWidget; }

 signals:
  void fileLoaded(const std::vector<S21Matrix> &points,
                  const std::set<segment> &faces, const int &modelId);
  void modelIdChanged(int modelId);
  void edgesColorChanged(const QColor &color);
  void vertixesColorChanged(const QColor &color);
  void backgroundColorChanged(const QColor &color);
  void vertixesSettingsChanged(const int &size, const int &typePoint);
  void edgesSettingsChanged(const int &width, const bool &isSolid);

 private slots:
  void exportModelTo();
  void sendToPrinter();

 private:
  QMenuBar *menuBar;
  QMenu *MainMenu;
  QMenu *PreferencesMenu;
  QMenu *RecordMenu;

  QString fileName;
  QString currentFilePath;

  QMenu *EdgesMenu;
  QAction *EdgesColorAction;
  QAction *EdgesTypeAction;

  QMenu *exportFileMenu;
  QAction *exportOBJAction;
  QAction *exportSTLAction;
  QAction *exportPLYAction;
  QAction *export3DSAction;
  QAction *exportDAEAction;

  QMenu *VertixesMenu;
  QAction *VertixesColorAction;
  QAction *VertixesTypeAction;

  QAction *exitAction;
  QAction *colorBackgroundAction;
  QAction *openFileAction;

  QWidgetAction *gifScreenshotAction;

  QMenu *Print3DMenu;
  QAction *openCuraAction;

  QMenu *RecentFilesMenu;
  QStringList recentFiles;
  const int maxRecentFiles = 5;

  Controller *controller;
  MyDialog *dialog;
  GifScreenshotWidget *gifScreenshotWidget;

  void setupMenu();
  void setupExportFilesConnections();
  void setupConnections();
  void setupBackgroundConnection();
  void setupSettingsConnections();
  void setupEdgesConnections();
  void setupVertixesConnections();
  void openFileMenu();

  bool prepareInputFile(const QString& originalFilePath, QString& outInputPath);
  void processModel(const QString& inputPath, const QString& originalFilePath);

  void updateRecentFiles(const QString &file);
  void rebuildRecentMenu();
  void loadRecentFilesFromSettings();

  QString prepareSTLFile(const QString &srcPath);
  bool lauchPrinterApp(const QString &winpath);

  const QString styleRecentFiles = R"(
    QMenu::item:selected {
      background-color: #3399FF; 
      color: white;  
    }
    QMenu {
      background-color: white;
      border: 1px solid gray;
    }
  )";

  const QIcon iconPrinter = QIcon(QCoreApplication::applicationDirPath()
                      +"/../View/icons/Logo_for_Cura_Software.png");
  const QIcon iconOBJ = QIcon(QCoreApplication::applicationDirPath()
                      +"/../View/icons/obj_icon.png");
  const QIcon iconSTL = QIcon(QCoreApplication::applicationDirPath()
                      +"/../View/icons/stl_icon.png");
  const QIcon iconPLY = QIcon(QCoreApplication::applicationDirPath()
                      +"/../View/icons/ply_icon.png");
  const QIcon icon3DS = QIcon(QCoreApplication::applicationDirPath()
                      +"/../View/icons/3ds_icon.png");
  const QIcon iconDAE = QIcon(QCoreApplication::applicationDirPath()
                      +"/../View/icons/dae_icon.png");
};

}  // namespace s21

#endif