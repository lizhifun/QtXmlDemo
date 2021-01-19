#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_Write_clicked();
    void on_btn_Delete_clicked();
    void on_btn_Read_clicked();

    void on_btn_Update_clicked();

private:
    Ui::MainWindow *ui;
    QString m_filePath="param.xml";

    //一些需要存储的变量
    int num=12;
    float dec=12.123;
    QString str="qwe";

};
#endif // MAINWINDOW_H
