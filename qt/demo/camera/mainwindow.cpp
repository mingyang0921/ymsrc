#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //若连接到多个摄像头设备，获取可使用的摄像头信息
    CameraInfoList = QCameraInfo::availableCameras();
    //遍历，将摄像头信息逐个添加到下拉列表当中
    QList<QCameraInfo>::const_iterator it;
    for(it=CameraInfoList.begin(); it!=CameraInfoList.end(); it++)
    {
        ui->deviceComboBox->addItem((*it).description());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::take_photo(int id, const QImage &image)
{
    //使用系统时间来命名图片的名称，时间是唯一的，图片名也是唯一的
    QDateTime dateTime(QDateTime::currentDateTime());
    QString time = dateTime.toString("yyyy-MM-dd-hh-mm-ss");
    //创建图片保存路径名
    QString filename = QString("./%1.jpg").arg(time);
    //保存一帧数据
    image.save(filename);
    //提示获取到了一帧数据
    QMessageBox::information(this, "提示：", "获取到一帧图片");
}

void MainWindow::on_pushButton_clicked()
{

    //获取到要打开的设备的名称
    QCameraInfo CameraInfo = CameraInfoList.at(ui->deviceComboBox->currentIndex());
    //创建摄像头对象
    Camera = new QCamera(CameraInfo);

    //CameraViewFinder = new QCameraViewfinder(this);

    //创建取景器并将其附在要显示的区域
    CameraViewFinder = new QCameraViewfinder(ui->CameraLabel);


    Camera->setViewfinder(CameraViewFinder);

    //CameraViewFinder->resize(300, 300);
    //调整摄像头数据显示的大小
    CameraViewFinder->resize(ui->CameraLabel->width(), ui->CameraLabel->height());
    //显示摄像头取景器
    CameraViewFinder->show();
    //开启摄像头
    Camera->start();

    //创建获取一帧数据对象
    CameraImageCapture = new  QCameraImageCapture(Camera);
    //关联图像获取信号
    connect(CameraImageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::take_photo);
}

void MainWindow::on_pushButton_2_clicked()
{
    CameraImageCapture->capture();
}
