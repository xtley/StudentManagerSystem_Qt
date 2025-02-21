#include "mainwindow.h"
#include "databasemanager.h"
#include <QApplication>

#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBaseManager::instance();
    QFile styleFile(":/style/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QString(styleFile.readAll());
        a.setStyleSheet(styleSheet); // 应用样式表
        styleFile.close();
    }
    else{
        qWarning()<<"Failed to load stylesheet:"<<styleFile.errorString();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
