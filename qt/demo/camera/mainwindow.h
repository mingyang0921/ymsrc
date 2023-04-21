#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:

    void take_photo(int id, const QImage &image);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    //摄像头对象指针
    QCamera* Camera;
    //摄像头的取景器
    QCameraViewfinder* CameraViewFinder;
    //存储摄像机设备信息
    QList<QCameraInfo> CameraInfoList;
    //记录摄像头内容
    QCameraImageCapture* CameraImageCapture;
};

#endif // MAINWINDOW_H
