#include "xmlutil.h"
#include <QtXml>

XMLUtil::XMLUtil(QObject *parent) : QObject(parent)
{

}

bool XMLUtil::XmlWrite(QString filename, QString nodename, QString itemname, QString itemvalue)
{
    //打开或创建文件
    QFile file(filename); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
        return false;

    QDomDocument doc;
    //写入xml头部
    QDomProcessingInstruction instruction; //添加处理命令
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    //添加根节点
    QDomElement root=doc.createElement("params");
    doc.appendChild(root);

    //添加第一个子节点及其子元素
    QDomElement book=doc.createElement(nodename);
    QDomElement title=doc.createElement(itemname); //创建子元素
    QDomText text; //设置括号标签中间的值
    text=doc.createTextNode(itemvalue);
    book.appendChild(title);
    title.appendChild(text);
    root.appendChild(book);

    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

    return true;
}

bool XMLUtil::XmlAdd(QString filename, QString nodename, QString itemname, QString itemvalue)
{
    //打开文件
    QFile file(filename); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return false;

    //增加一个一级子节点以及元素
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomElement book=doc.createElement(nodename);
    QDomElement title=doc.createElement(itemname);
    QDomText text;
    text=doc.createTextNode(itemvalue);
    title.appendChild(text);
    book.appendChild(title);
    root.appendChild(book);

    if(!file.open(QFile::WriteOnly|QFile::Truncate)) //先读进来，再重写，如果不用truncate就是在后面追加内容，就无效了
        return false;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

    return true;
}

bool XMLUtil::XmlRemove(QString filename, QString nodename, QString itemname)
{
    //打开文件
    QFile file(filename); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return false;

    //删除一个一级子节点及其元素，外层节点删除内层节点于此相同
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();  //一定要记得关掉啊，不然无法完成操作


    QDomElement root=doc.documentElement();
    QDomNodeList list=doc.elementsByTagName(nodename); //由标签名定位
    for(int i=0;i<list.count();i++)
    {
        QDomNode node=list.at(i).firstChild();
        if(node.nodeName() == itemname)  //以属性名定位，类似于hash的方式，warning：这里仅仅删除一个节点，其实可以加个break
            root.removeChild(list.at(i));
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return false;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

    return true;
}

void XMLUtil::XmlUpdate(QString filename, QString nodename, QString itemname, QString itemvalue)
{
    //打开文件
    QFile file(filename); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomNodeList list=root.elementsByTagName(nodename);
    for(int i=0;i<list.count();i++)
    {
        QDomNode node=list.at(i).firstChild();
        if(node.nodeName() == itemname)
        {
            QDomNode oldnode=node.firstChild(); //标签之间的内容作为节点的子节点出现
            node.firstChild().setNodeValue(itemvalue);
            QDomNode newnode=node.firstChild();
            node.replaceChild(newnode,oldnode);
        }
    }

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

QMap<QString, QString> *XMLUtil::XmlRead(QString filename)
{
    //打开或创建文件
    QFile file(filename); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
        return NULL;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return NULL;
    }
    file.close();

    QMap<QString,QString>* paramMap = new QMap<QString,QString>();
    paramMap->clear();

    QDomElement root=doc.documentElement(); //返回根节点
    QDomNode node=root.firstChild(); //获得第一个子节点
    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            QDomElement e=node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
            QDomNodeList list=e.childNodes();
            for(int i=0;i<list.count();i++) //遍历子元素，count和size都可以用,可用于标签数计数
            {
                QDomNode n=list.at(i);
                if(node.isElement())
                {
                    paramMap->insert(n.nodeName(),n.toElement().text());
                }
            }
        }
        node=node.nextSibling(); //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
    }

    file.close();
    return paramMap;
}

QMap<QString, QString> *XMLUtil::XmlRead(QString filename, QString nodename)
{
    //打开或创建文件
    QFile file(filename); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
        return NULL;

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return NULL;
    }
    file.close();

    QMap<QString,QString>* paramMap = new QMap<QString,QString>();
    paramMap->clear();

    QDomElement root=doc.documentElement(); //返回根节点
    QDomNode node=root.firstChild(); //获得第一个子节点
    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            QDomElement e=node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
            if(e.nodeName() == nodename)
            {
                QDomNodeList list=e.childNodes();
                for(int i=0;i<list.count();i++) //遍历子元素，count和size都可以用,可用于标签数计数
                {
                    QDomNode n=list.at(i);
                    if(node.isElement())
                    {
                        paramMap->insert(n.nodeName(),n.toElement().text());
                    }
                }
            }
        }
        node=node.nextSibling(); //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
    }

    file.close();
    return paramMap;
}

QString XMLUtil::XmlRead(QString filename, QString nodename, QString itemname)
{
    //打开或创建文件
    QFile file(filename); //相对路径、绝对路径、资源路径都行
    if(!file.open(QFile::ReadOnly))
        return "";

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return "";
    }
    file.close();

    QDomElement root=doc.documentElement(); //返回根节点
    QDomNode node=root.firstChild(); //获得第一个子节点
    while(!node.isNull())  //如果节点不空
    {
        if(node.isElement()) //如果节点是元素
        {
            QDomElement e=node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
            if(e.nodeName() == nodename)
            {
                QDomNodeList list=e.childNodes();
                for(int i=0;i<list.count();i++) //遍历子元素，count和size都可以用,可用于标签数计数
                {
                    QDomNode n=list.at(i);
                    if(node.isElement())
                    {
                        if(n.nodeName() == itemname)
                        {
                            file.close();
                            return n.toElement().text();
                        }
                    }
                }
            }
        }
        node=node.nextSibling(); //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
    }
    return "";
}

bool XMLUtil::XmlItemExist(QString filename, QString nodename, QString itemname)
{
    //打开文件
    QFile file(filename); //相对路径、绝对路径、资源路径都可以
    if(!file.open(QFile::ReadOnly))
        return false;

    //更新一个标签项,如果知道xml的结构，直接定位到那个标签上定点更新
    //或者用遍历的方法去匹配tagname或者attribut，value来更新
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomNodeList list=root.elementsByTagName(nodename);
    for(int i=0;i<list.count();i++)
    {
        QDomNode node=list.at(i).firstChild();
        if(node.nodeName() == itemname)
        {
            file.close();
            return true;
        }
    }
    return false;
}
