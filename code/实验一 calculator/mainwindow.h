#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    //将表达式转化为后缀表达式
    QString inToPost(QString infix) throw(const char*);
    //计算后缀表达式的结果
    double compute(QString s) throw(const char*);

    Ui::MainWindow *ui;

    bool waitForOperand;//标记是否等待一个操作数
    QString error;
    bool calculate(double operand, QString pendingOperator);
    //终止运算，清除数据，报错
    void abortOperation();
    //连接信号和槽
    void connectSlots();
    //储存运算符
    QString pendingOperator;
    //储存运算结果
    double result;

private slots:
    void on_clearBtn_clicked();
    void on_clearAllBtn_clicked();
    void on_equalBtn_clicked();
    void digitClicked();
    void on_signBtn_clicked();
    void operatorClicked();
    void on_pointBtn_clicked();
    void setShortcutKeys();
    void on_pushButtonSin_clicked();
    void on_pushButtonCos_clicked();
    void on_pushButtonTan_clicked();
    void on_pushButtonLn_clicked();
    void on_pushButtonSqrt_clicked();
    void on_pushButtonArcsin_clicked();

};



#endif // MAINWINDOW_H
