#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString OpenFile,OpenFilePath;
    QStringList files;
    double wtimes,htimes;
    int choosePath;
    int Play_fl;
    QTimer *timer;
//    QString pic_path;

private slots:
    void on_action_pre_triggered();

    void on_action_choose_triggered();

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

    void on_action_clear_triggered();

    void on_action_next_triggered();

    void on_action_left_triggered();

    void on_action_right_triggered();

    void on_action_big_triggered();

    void on_action_small_triggered();

    void on_action_return_triggered();

    void showmaxpic(QString place);

    void on_action_all_triggered();

    void on_action_play_triggered();

    void on_shuffling_timer();

    void on_action_edit_triggered();

private:
    Ui::MainWindow *ui;
    //void connectSlots();
    //void MainWindow::selectFile()

};

#endif // MAINWINDOW_H
