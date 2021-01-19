#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include "xmlutil.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_Write_clicked()
{
    if(!QFile::exists(m_filePath))//如果文件不存在，则调用XMLUtil::XmlWrite，第一次创建一个文件
    {
        XMLUtil::XmlWrite(m_filePath,"param","num",QString::number(num));
    }
    else//如果文件存在，需要判断当前项是否存在，如果存在，则更新；如果不存在，则追加
    {
        //写num变量
        if(XMLUtil::XmlItemExist(m_filePath,"param","num"))
        {
            XMLUtil::XmlUpdate(m_filePath,"param","num",QString::number(num));
        }
        else
        {
            XMLUtil::XmlAdd(m_filePath,"param","num",QString::number(num));
        }
    }
    //dec变量
    if(XMLUtil::XmlItemExist(m_filePath,"param","dec"))
    {
        XMLUtil::XmlUpdate(m_filePath,"param","dec",QString::number(dec));
    }
    else
    {
        XMLUtil::XmlAdd(m_filePath,"param","dec",QString::number(dec));
    }
    //str变量
    if(XMLUtil::XmlItemExist(m_filePath,"param","str"))
    {
        XMLUtil::XmlUpdate(m_filePath,"param","str",str);
    }
    else
    {
        XMLUtil::XmlAdd(m_filePath,"param","str",str);
    }
}


void MainWindow::on_btn_Delete_clicked()
{
    //删除dec变量
    XMLUtil::XmlRemove(m_filePath,"param","dec");
    qDebug() << QStringLiteral("删除项 dec");
}

void MainWindow::on_btn_Read_clicked()
{
    //读出所有值
    QMap<QString,QString> *res = XMLUtil::XmlRead(m_filePath);
    //遍历读出的内容并打印出来
    QMap<QString, QString>::iterator iter = res->begin();
    while (iter != res->end())
    {
         qDebug() << QStringLiteral("读取XML内容: ") << iter.key() << ":" << iter.value();
         iter++;
    }
}

void MainWindow::on_btn_Update_clicked()
{
    //将str值更新为 "what"
    XMLUtil::XmlUpdate(m_filePath,"param","str","what");
    qDebug() << QStringLiteral("将 str 更新为 what");
}
