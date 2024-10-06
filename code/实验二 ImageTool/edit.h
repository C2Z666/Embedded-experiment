#ifndef EDIT_H
#define EDIT_H

#include <QDialog>
#include <opencv2/opencv.hpp>
namespace Ui {
class Edit;
}

class Edit : public QDialog
{
    Q_OBJECT

public:
    explicit Edit(QString path,QWidget *parent = nullptr);
    ~Edit();

    QString pic_path;


private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_activated(int index);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_pushButton_3_clicked(bool checked);

    //void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Edit *ui;

    cv::Mat adJustContrast(const cv::Mat& image,double alpha);
    void displayImage(const cv::Mat& image);
};

#endif // EDIT_H
