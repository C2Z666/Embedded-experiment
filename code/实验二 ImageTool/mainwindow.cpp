#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QTransform>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include "edit.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer;
//    QListWidget *imgList = new QListWidget(this);
    QListWidget *imgList = ui->listWidget_2;
    imgList->setIconSize(QSize(64,64));
    imgList->setViewMode(QListView::IconMode);
    imgList->setFlow(QListView::LeftToRight);
    imgList->setResizeMode(QListView::Adjust);
    imgList->setMovement(QListView::Static);

    imgList->setStyleSheet("QListWidget::item:selected{background:lightblue;color:green;}"
                           "QListWidget::item:hover{background:lightgray;color:green;}"
                           "QListWidget::item{padding-left:0px;padding-top:20px;padding-bottom:4px;color:black}"
                           );
    wtimes=1;
    htimes=1;
    choosePath=0;
    //connectSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//显示缩略图全屏图片
void MainWindow::showmaxpic(QString place)
{
    if(!choosePath)return;
    //qDebug()<<place<<endl;
    QWidget *picshow=new QWidget;//无this，为新建界面
    picshow->setWindowFlags(Qt::FramelessWindowHint);
    picshow->setWindowModality(Qt::ApplicationModal);
    picshow->setAttribute(Qt::WA_DeleteOnClose);//若是关闭界面，则彻底释放资源
    picshow->resize(1280,1024);
    //显示双击的缩略图放大图片
    QLabel *pic=new QLabel(picshow);//在picshow上面新建控件
    QImage *img=new QImage;
    img->load(place);
    int size_w = 1280;//展示框宽
    int size_h = 1024;//展示框高
    int width = img->width();  //实际图片宽
    int height = img->height();//实际图片高
    int scaleWidth = size_w;//最终计算图片宽度
    int scalHeight = size_h;//最终计算图片高度
    //qDebug()<<"1"<<endl;
    if(width > size_w){//图片本身宽度大于展示框宽度，等比例缩放
        scaleWidth = size_w;
        scalHeight = scaleWidth *height /width;
//        qDebug() << scalHeight << scaleWidth;
        if(scalHeight > size_h){//缩放后高度大于展示框，根据展示框高度等比例缩放
            scalHeight = size_h;
            scaleWidth = scalHeight * width/height;
        }
        pic->resize(scaleWidth,scalHeight);
        pic->move((size_w-scaleWidth)/2,(size_h-scalHeight)/2);
        //qDebug()<<"1"<<endl;
    }
    else if(height > size_h){//图片本身高度大于展示框，等比例缩放
        scalHeight = size_h;
        scaleWidth = scalHeight * size_w/height;
//        qDebug() << scalHeight << scaleWidth;
        if(scaleWidth > size_w){//缩放后宽度大于展示框，根据展示框宽度等比例缩放
            scaleWidth = size_w;
            scalHeight = scaleWidth *height /width;
        }
        pic->resize(scaleWidth,scalHeight);
        pic->move((size_w-scaleWidth)/2,(size_h-scalHeight)/2);
        //qDebug()<<"2"<<endl;
    }
    else{//符合展示框高度和宽度，可以直接使用
        pic->resize(width,height);
        pic->move((size_w-width)/2,(size_h-height)/2);
        //qDebug()<<"3"<<endl;
    }
    //qDebug()<<"2"<<endl;
    img->scaled(pic->size(),Qt::KeepAspectRatio);//设置图片模式//Qt::IgnoreAspectRatio//Qt::KeepAspectRatio
    pic->setScaledContents(true);//使能图片缩放填充QLabel模式
    pic->setPixmap(QPixmap::fromImage(*img));
//    pic->show();
    delete img;//必须释放内存，否则运行一段时间内存爆满程序崩溃
//    pic->setAlignment(Qt::AlignCenter);//将图片放在label中心,用缩放了就不需要了
    //返回按键
    QPushButton *button_cancel=new QPushButton(picshow);
    //右上角有一个小按键点击退出
//    button_cancel->resize(61,61);
//    button_cancel->move(1840,10);
//    button_cancel->setStyleSheet("border-image: url(:/PIC/icon/放大.png);");
//    qDebug()<<"3"<<endl;
    //全屏按键点击退出
    button_cancel->resize(1280,1024);
    button_cancel->move(0,0);
    button_cancel->setStyleSheet("border-image: url(:/icons/nothing.png);");
    connect(button_cancel,&QPushButton::clicked,[=](){picshow->close();});
    picshow->show();
    //qDebug()<<"4"<<endl;
}

void MainWindow::on_action_pre_triggered() // previous
{
    if(!choosePath)return;
    QListWidget *imgList = ui->listWidget_2;
    int idx=imgList->currentRow()-1;
//    qDebug()<<"1:"<<idx<<endl;
    if(idx<0){
        idx = files.size()-1;
    }
    imgList->setCurrentRow(idx);
//    qDebug()<<"2:"<<idx<<endl;
    QString path = OpenFilePath + "/"+ files.at(idx);
    QImage image;
    if(image.load(path))
    {
        QPixmap img=QPixmap::fromImage(image);
        img = img.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->labelPic->setPixmap(img);
    }
    wtimes=1;
    htimes=1;
}

void MainWindow::on_action_next_triggered()
{
    if(!choosePath)return;
    QListWidget *imgList = ui->listWidget_2;
    int idx=imgList->currentRow()+1;
//    qDebug()<<"1:"<<idx<<endl;
    if(idx>=files.size()){
        idx = 0;
    }
    imgList->setCurrentRow(idx);
//    qDebug()<<"2:"<<idx<<endl;
    QString path = OpenFilePath + "/"+ files.at(idx);
    QImage image;
    if(image.load(path))
    {
        QPixmap img=QPixmap::fromImage(image);
        img = img.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->labelPic->setPixmap(img);
    }
    wtimes=1;
    htimes=1;
}

void MainWindow::on_action_choose_triggered() // select
{
    OpenFile = QFileDialog::getExistingDirectory(this,"choose Dir","./",QFileDialog::ShowDirsOnly);
    QFileInfo OpenFileInfo;
    OpenFileInfo = QFileInfo(OpenFile);
    OpenFilePath = OpenFileInfo.filePath();

    ui->labelPath_2->setText(OpenFilePath);

    QDir dir(OpenFilePath);

    QStringList namefiles;
    namefiles<<"*.png"<<"*.jpg"<<"*.bmp";
    QListWidget *imgList = ui->listWidget_2;
    files = dir.entryList(namefiles,QDir::Files|QDir::Readable,QDir::Name);

    for(int i=0;i<files.size();++i)
    {
        QListWidgetItem *newItem = new QListWidgetItem(QIcon(QPixmap(OpenFilePath+"/"+files.at(i)).scaled(125,125)),files.at(i));

        QFileInfo fi = files.at(i);

        newItem->setText(fi.baseName());

        newItem->setTextAlignment(Qt::AlignHCenter);


        imgList->addItem(newItem);
    }
    choosePath=1;
}

void MainWindow::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    QListWidget *imgList = ui->listWidget_2;
//    qDebug()<<imgList->currentRow()<<" all:"<<files.size()<<endl;
//    for(int i=0;i<files.size();i++){
//        qDebug()<<files.at(i)<<endl;
//    }
    QString path = OpenFilePath + "/"+ files.at(imgList->currentRow());
//    qDebug()<<path<<endl;
    QImage image;
    if(image.load(path))
    {
        QPixmap img=QPixmap::fromImage(image);
        img = img.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->labelPic->setPixmap(img);
    }
}

void MainWindow::on_action_clear_triggered()
{
    OpenFilePath = "";
    ui->labelPath_2->clear();
    ui->labelPic->clear();
    ui->listWidget_2->clear();
    choosePath=0;
}


void MainWindow::on_action_left_triggered()
{
    if(!choosePath)return;
    QTransform transform;
//    QPixmap pix2 = ui->labelPic->pixmap()->transformed(transform.scale(-1,1));
    QPixmap pix2 = ui->labelPic->pixmap()->transformed(transform.rotate(90));
    ui->labelPic->setPixmap(pix2);
}



void MainWindow::on_action_right_triggered()
{
    if(!choosePath)return;
    QTransform transform;
//    QPixmap pix2 = ui->labelPic->pixmap()->transformed(transform.scale(-1,1));
    QPixmap pix2 = ui->labelPic->pixmap()->transformed(transform.rotate(-90));
    ui->labelPic->setPixmap(pix2);
}

void MainWindow::on_action_big_triggered()
{
    if(!choosePath)return;
    if(wtimes/2>5)return;
    QTransform transform;
    QPixmap pix2 = ui->labelPic->pixmap()->transformed(transform.scale(2,2));
    ui->labelPic->setPixmap(pix2);
    wtimes*=2;
    htimes*=2;
}

void MainWindow::on_action_small_triggered()
{
    if(!choosePath)return;
    if(wtimes/2<0.2)return;
    QTransform transform;
    QPixmap pix2 = ui->labelPic->pixmap()->transformed(transform.scale(0.5,0.5));
    ui->labelPic->setPixmap(pix2);
    wtimes/=2;
    htimes/=2;
}

void MainWindow::on_action_return_triggered()
{
    if(!choosePath)return;
    QListWidget *imgList = ui->listWidget_2;
    QString path = OpenFilePath + "/"+ files.at(imgList->currentRow());
    QImage image;
    if(image.load(path))
    {
        QPixmap img=QPixmap::fromImage(image);
        img = img.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->labelPic->setPixmap(img);
    }

    wtimes=1;
    htimes=1;
}

void MainWindow::on_action_all_triggered()
{
    if(!choosePath)return;
    QListWidget *imgList = ui->listWidget_2;
    QString path = OpenFilePath + "/"+ files.at(imgList->currentRow());
    showmaxpic(path);
}

void MainWindow::on_action_play_triggered()
{
    if(!choosePath)return;
    QListWidget *imgList = ui->listWidget_2;
    int idx = imgList->currentRow();


    Play_fl+=1;Play_fl%=2;
    if(Play_fl)
    {
        connect(timer, SIGNAL(timeout()),this, SLOT(on_shuffling_timer()));

        timer->start(2000);
    }
    else timer->stop();

}

void MainWindow::on_shuffling_timer()
{
    if(!choosePath)return;
    QListWidget *imgList = ui->listWidget_2;
    int idx = imgList->currentRow();
    idx += 1;
    idx%=files.size();
    QImage image;
    image.load(OpenFilePath + "/"+ files.at(idx));
    QPixmap img=QPixmap::fromImage(image);
    img = img.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->labelPic->setPixmap(img);
    imgList->setCurrentRow(idx);
}


void MainWindow::on_action_edit_triggered()
{
    if(!choosePath)return;
    QListWidget *imgList = ui->listWidget_2;
    int idx = imgList->currentRow();
    QString pic_path = OpenFilePath + "/"+ files.at(idx);


    Edit *edit = new Edit(pic_path);
    this->hide();
//    edit->setMsg(pic_path);
    edit->show();

}
