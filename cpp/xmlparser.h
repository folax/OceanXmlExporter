#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QFile>
#include <QDomDocument>
#include "mainWindow.h"


class QDomDocument;

class XmlParser : public QObject
{
    Q_OBJECT
public:

    explicit XmlParser();
    ~XmlParser();


    //work with XML
    bool FirmIsExists(const QString&);
    void UpdateFirmData(const QString& firmName, const FirmData& firmData);

    //read XML
    bool readAllDataFromXML();

    //add, remove item to XML
    bool addItemToXML(const FirmData& data);
    bool removeFirmFromXML(const QString&);

    QVector<FirmData> getFirmsData() { return m_dataFromXML; }
    FirmData getCurrentFirmData(QString& firmName);

signals:
    void sigDataFromXml(QStringList);

private:
    QString m_FileName;
    QFile m_xmlFile;

    //QDomDocument m_xmlDomFile;
    QVector<FirmData> m_dataFromXML;

    // Create xml file and xml structure;
    void createXmlFile();
};

#endif // XMLPARSER_H
