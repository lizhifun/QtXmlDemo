#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <QObject>
#include <QXmlReader>
#include <QMap>

class XMLUtil : public QObject
{
    Q_OBJECT
public:
    explicit XMLUtil(QObject *parent = nullptr);
    //写XML文件
    static bool XmlWrite(QString filename, QString nodename, QString itemname, QString itemvalue);
    //增加节点内容
    static bool XmlAdd(QString filename, QString nodename, QString itemname, QString itemvalue);
    //删除项
    static bool XmlRemove(QString filename, QString nodename, QString itemname);
    //更新项
    static void XmlUpdate(QString filename, QString nodename, QString itemname, QString itemvalue);
    //读取XML文件中所有内容
    static QMap<QString,QString>* XmlRead(QString filename);
    //读取节点下所有项
    static QMap<QString,QString>* XmlRead(QString filename,QString nodename);
    //读取项内容
    static QString XmlRead(QString filename,QString nodename,QString itemname);
    //判断项是否存在
    static bool XmlItemExist(QString filename,QString nodename,QString itemname);

signals:

};

#endif // XMLUTIL_H
