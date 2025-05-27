#include <QString>
#include <QTemporaryFile>
#include <QDir>


namespace s21 {
class ModelIO {
public:
    static bool convertStlToObj(const QString& stlPath, QString& outObjPath);
    static bool exportModel(const QString& srcPath, const QString &format, QString& destPath);
};
}
