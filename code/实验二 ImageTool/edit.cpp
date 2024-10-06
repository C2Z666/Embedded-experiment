#include "edit.h"
#include "ui_edit.h"
#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
Edit::Edit(QString path,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit)
{
    pic_path=path;
    ui->setupUi(this);
    pic_path = path;
    QImage image;
    image.load(path);
    QPixmap img=QPixmap::fromImage(image);
//    qDebug()<<path<<endl;
    img = img.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label_show->setPixmap(img);

    connect(ui->horizontalSlider, &QSlider::valueChanged, this, [this](int value)
    {
//        qDebug()<<value<<endl;
        double alpha = value;
        cv::Mat image = cv::imread(pic_path.toStdString());
//        QImage img = ui->label_show->pixmap()->toImage();
//        img = img.convertToFormat(QImage::Format_RGB888);
//        cv::Mat image = cv::Mat(img.height(), img.width(), CV_8UC3, img.bits(), img.bytesPerLine());
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        cv::Mat contrastImage = adJustContrast(image, alpha);

        displayImage(contrastImage);
    });

    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, [this](int value)
    {
        double alpha = value;
        cv::Mat image = cv::imread(pic_path.toStdString());
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        image.convertTo(image,-1,1,alpha);
        displayImage(image);
    });
}

Edit::~Edit()
{
    delete ui;
}

void Edit::on_pushButton_clicked()
{

}

void Edit::displayImage(const cv::Mat& image)
{
    QImage qimg(image.data, image.cols, image.rows, image.step , QImage::Format_RGB888);
    qimg = qimg.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label_show->setPixmap(QPixmap::fromImage(qimg));
}
cv::Mat Edit::adJustContrast(const cv::Mat& image,double alpha)
{
    cv::Mat newImage = cv::Mat::zeros(image.size(), image.type());

    image.convertTo(newImage, -1, alpha, 0);
    return newImage;
}


void Edit::on_comboBox_currentIndexChanged(int index)
{
    qDebug()<<index<<endl;
}

void Edit::on_comboBox_activated(int index)
{
    if(index==0)
    {
        QImage img = ui->label_show->pixmap()->toImage();

        img = img.convertToFormat(QImage::Format_RGB888);
        cv::Mat image = cv::Mat(img.height(), img.width(), CV_8UC3, img.bits(), img.bytesPerLine());

        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

        double contrastFactor = 1.5;
        cv::Mat contrastImage = adJustContrast(image, contrastFactor);

        displayImage(contrastImage);
    }


}

void Edit::on_horizontalSlider_valueChanged(int value)
{
;
}

void Edit::on_horizontalSlider_2_valueChanged(int value)
{

}

void Edit::on_pushButton_3_clicked(bool checked)
{
    QString fileName = QFileDialog::getSaveFileName(this,"save","","Image (*.png *.jpg *.bmp)");
    if(fileName.isEmpty()){
        return ;
    }
    QImage img = ui->label_show->pixmap()->toImage();
    img.save(fileName);
}
