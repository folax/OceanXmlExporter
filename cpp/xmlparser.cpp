#include "xmlParser.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>


XmlParser::XmlParser() : m_FileName("firms_data.xml"), m_xmlFile(QApplication::applicationDirPath() + "/" + m_FileName)
{
    if (!m_xmlFile.open(QIODevice::ReadOnly))
    {
        this->createXmlFile();
    }
    m_xmlFile.close();
}

void XmlParser::createXmlFile()
{
    QDomDocument doc("Firms");
    QDomElement domElement = doc.createElement("Firms-to-parse-list");
    doc.appendChild(domElement);

    if(m_xmlFile.open(QIODevice::WriteOnly))
    {
        QTextStream(&m_xmlFile) << doc.toString();
        m_xmlFile.close();
    }
    qDebug() << "Create XML structure!";
}

bool XmlParser::addItemToXML(const FirmData &data)
{
    // Checking for data is correct;
    if(data.FirmName.isEmpty() || data.FirmOkpo.isEmpty() || data.FirmInn.isEmpty()
            || data.FirmDateInn.isEmpty() || data.FirmAddress.isEmpty() || data.FirmTelephone.isEmpty()
            || data.FirmBank.isEmpty() || data.FirmBankAccount.isEmpty() || data.FirmCertificate.isEmpty())
    {
        QMessageBox::warning(NULL, "Не все поля заполненнны", "Заполните все поля.");
        return false;
    }
    else
    {
        if(!FirmIsExists(data.FirmName))
        {
            if(!m_xmlFile.open(QIODevice::ReadWrite))
            {
                qDebug() << "Coul'd not open file!";
                return false;
            }
            QDomDocument doc;
            if(!doc.setContent(&m_xmlFile))
            {
                qDebug( "Failed to parse the file into a DOM tree." );
                createXmlFile();
                doc.setContent(&m_xmlFile);
            }
            QDomElement root = doc.documentElement();
            QDomElement newFirm = doc.createElement("firm");
            newFirm.setAttribute("FirmName",        data.FirmName);
            newFirm.setAttribute("FirmOkpo",        data.FirmOkpo);
            newFirm.setAttribute("FirmInn",         data.FirmInn);
            newFirm.setAttribute("FirmDateInn",     data.FirmDateInn);
            newFirm.setAttribute("FirmAddress",     data.FirmAddress);
            newFirm.setAttribute("FirmTelephone",   data.FirmTelephone);
            newFirm.setAttribute("FirmBank",        data.FirmBank);
            newFirm.setAttribute("FirmBankAccount", data.FirmBankAccount);
            newFirm.setAttribute("FirmCertificate", data.FirmCertificate);

            root.appendChild(newFirm);

            m_xmlFile.close();

            m_xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
            QTextStream ts(&m_xmlFile);
            ts.setCodec("utf-8");
            ts << doc.toString();
            m_xmlFile.close();
            readAllDataFromXML();
            QMessageBox::warning(NULL, "Предпирятие добавлено!", "Операция успешно выполнена.");
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool XmlParser::removeFirmFromXML(const QString& firmName)
{
    if(!m_xmlFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Coul'd not open file!";
        return false;
    }

    QDomDocument doc;
    if(!doc.setContent(&m_xmlFile))
    {
        qDebug( "Failed to parse the file into a DOM tree." );
        createXmlFile();
        doc.setContent(&m_xmlFile);
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(0, "Удаление предприятия", "Вы действительно хотите удалить предприятие ?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {

        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild(); //product
        while(!n.isNull())
        {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(!e.isNull())
            {
                if (e.tagName() == "firm" && e.attribute("FirmName") == firmName)
                {
                    docElem.removeChild(n);
                }
            }
            n = n.nextSibling();
        }
        m_xmlFile.close();
        m_xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

        QTextStream ts(&m_xmlFile);
        ts.setCodec("utf-8");
        ts << doc.toString();
        m_xmlFile.close();
        readAllDataFromXML();
        return true;
    }
    else
    {
        m_xmlFile.close();
        return false;
    }
}

FirmData XmlParser::getCurrentFirmData(QString& firmName)
{
    for(int i(0); i < m_dataFromXML.size(); ++i)
    {
        if(m_dataFromXML.at(i).FirmName == firmName)
        {
            return m_dataFromXML.at(i);
        }
    }
}


bool XmlParser::readAllDataFromXML()
{
    qDebug() << "readAllDataFromXML()";
    if(!m_xmlFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Coul'd not open file! : readAllDataFromXML()";
        return false;
    }

    QDomDocument doc;
    if(!doc.setContent(&m_xmlFile))
    {
        qDebug( "Failed to parse the file into a DOM tree. : readAllDataFromXML()" );
        createXmlFile();
        doc.setContent(&m_xmlFile);
    }


    //clear data file
    m_dataFromXML.clear();
    //read data from file
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    FirmData tmpData;
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            if (e.tagName() == "firm")
            {
                tmpData.FirmName = e.attribute("FirmName");
                tmpData.FirmOkpo = e.attribute("FirmOkpo");
                tmpData.FirmInn = e.attribute("FirmInn");
                tmpData.FirmDateInn = e.attribute("FirmDateInn");
                tmpData.FirmAddress = e.attribute("FirmAddress");
                tmpData.FirmTelephone = e.attribute("FirmTelephone");
                tmpData.FirmBank = e.attribute("FirmBank");
                tmpData.FirmBankAccount = e.attribute("FirmBankAccount");
                tmpData.FirmCertificate = e.attribute("FirmCertificate");
            }
        }
        n = n.nextSibling();
        m_dataFromXML.push_back(tmpData);
    }
    m_xmlFile.close();

    QStringList firmNames;
    if(!m_dataFromXML.isEmpty())
    {
        for(auto &it : m_dataFromXML)
            firmNames.append(it.FirmName);
    }
    qDebug() << "Size: "<< m_dataFromXML.size() << "Names: " << firmNames;
    emit sigDataFromXml(firmNames);
    return true;
}


XmlParser::~XmlParser()
{

}

bool XmlParser::FirmIsExists(const QString& firm_name)
{
    if(!m_xmlFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Coul'd not open file!";
        return false;
    }

    QDomDocument doc;
    if(!doc.setContent(&m_xmlFile))
    {
        qDebug( "Failed to parse the file into a DOM tree." );
        createXmlFile();
        doc.setContent(&m_xmlFile);
    }

    //qDebug() << "*** productExists() *** :" << firm_name;

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            if (e.tagName() == "firm")
            {
                if (e.attribute("FirmName") == firm_name)
                {
                    QMessageBox::warning(NULL, "Такое предприятие уже существует", "Выбирите другое название пожалуйста!");
                    m_xmlFile.close();
                    return true;
                }
            }
        }
        n = n.nextSibling();
    }
    m_xmlFile.close();
    return false;
}

void XmlParser::UpdateFirmData(const QString &firmName, const FirmData &firmData)
{
    // Checking for input values;
    if(firmName != firmData.FirmName)
    {
        QMessageBox::warning(NULL, "Имена на совпадают", "Для обновления данных предприятия названия двух верхних полей должны совпадать!");
        return;
    }
    else
    {
        if(!m_xmlFile.open(QIODevice::ReadWrite))
        {
            qDebug() << "Coul'd not open file!";
            return;
        }

        QDomDocument doc;
        if(!doc.setContent(&m_xmlFile))
        {
            qDebug( "Failed to parse the file into a DOM tree." );
            createXmlFile();
            doc.setContent(&m_xmlFile);
        }

        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild(); //product
        while(!n.isNull())
        {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(!e.isNull())
            {
                if (e.tagName() == "firm" && e.attribute("FirmName") == firmName)
                {
                    docElem.removeChild(n);
                }
            }
            n = n.nextSibling();
        }

        //add element with changes
        QDomElement newFirm = doc.createElement("firm");
        newFirm.setAttribute("FirmName",        firmData.FirmName);
        newFirm.setAttribute("FirmOkpo",        firmData.FirmOkpo);
        newFirm.setAttribute("FirmInn",         firmData.FirmInn);
        newFirm.setAttribute("FirmDateInn",     firmData.FirmDateInn);
        newFirm.setAttribute("FirmDateInn",     firmData.FirmDateInn);
        newFirm.setAttribute("FirmAddress",     firmData.FirmAddress);
        newFirm.setAttribute("FirmTelephone",   firmData.FirmTelephone);
        newFirm.setAttribute("FirmBank",        firmData.FirmBank);
        newFirm.setAttribute("FirmBankAccount", firmData.FirmBankAccount);
        newFirm.setAttribute("FirmCertificate", firmData.FirmCertificate);
        docElem.appendChild(newFirm);

        m_xmlFile.close();
        m_xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

        QTextStream ts(&m_xmlFile);
        ts.setCodec("utf-8");
        ts << doc.toString();

        m_xmlFile.close();
        readAllDataFromXML();
    }
}






