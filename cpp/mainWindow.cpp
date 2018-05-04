#include "mainWindow.h"
#include "partnersEditWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QComboBox>
#include <QMessageBox>


OceanXmlExporter::OceanXmlExporter(QWidget *parent) : QDialog(parent), m_xmlDoc("document")
{
    setWindowFlags(Qt::Window);
    setWindowTitle(tr("Подпись XML файлов"));

    this->resize(350, 214);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Load XML file;
    loadXmlLayout_1 = new QVBoxLayout;
    loadXmlFileBtn = new QPushButton(tr("Загрузить данные из файла"));

    loadXmlFileStatus = new QLabel(tr("Статус файла:"));
    signXmlFileBtn = new QPushButton(tr("Подписать файл"));
    signXmlFileBtn->setEnabled(false);

    loadXmlLayout_1->addWidget(loadXmlFileBtn);
    loadXmlLayout_1->addWidget(loadXmlFileStatus);
    loadXmlLayout_1->addWidget(signXmlFileBtn);

    loadXmlGroup_1 = new QGroupBox(tr("Загрузка данных из файла"));
    loadXmlGroup_1->setLayout(loadXmlLayout_1);

    loadXmlLayout_2 = new QVBoxLayout;
    loadXmlGroup_2 = new QGroupBox(tr("Выбор предприятия для подписи:"));
    editFirmsBtn = new QPushButton(tr("Редактировать предприятия"));
    cancelBtn = new QPushButton(tr("Отмена"));
    firmsSelectorComboBox = new QComboBox();
    loadXmlLayout_2->addWidget(firmsSelectorComboBox);
    loadXmlLayout_2->addWidget(editFirmsBtn);
    loadXmlLayout_2->addWidget(cancelBtn);


    loadXmlGroup_2->setLayout(loadXmlLayout_2);


    mainLayout->addWidget(loadXmlGroup_1);
    mainLayout->addWidget(loadXmlGroup_2);
    mainLayout->setAlignment(Qt::AlignTop);


    this->setLayout(mainLayout);

    // Connect
    connect(loadXmlFileBtn, &QPushButton::clicked, this, &slotOpenFile);
    connect(editFirmsBtn,   &QPushButton::clicked, this, &slotEditParntners);
    connect(cancelBtn,      &QPushButton::clicked, this, &close);
    connect(signXmlFileBtn, &QPushButton::clicked, this, &slotSetFirmSignature);

    // Partners gui window initializtion;
    partnersWindow = new PartnersEditWindow();
    getFirmsListData();
}

void OceanXmlExporter::getFirmsListData()
{
    QVector<FirmData> firmVec = partnersWindow->getFirmListData();
    if(!firmVec.isEmpty())
    {
        firmsSelectorComboBox->clear();
        for(int i(0); i < firmVec.size(); ++i)
            firmsSelectorComboBox->addItem(firmVec.at(i).FirmName);
    }
}

void OceanXmlExporter::slotOpenFile()
{
    m_pathToFile = QFileDialog::getOpenFileName(this,
                                                tr("Open Xml file"), "D:/", tr("Xml Files (*.xml)"));
    if(!m_pathToFile.isEmpty())
    {
        QFile xmlFile(m_pathToFile);
        if(!xmlFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Can't open file: " << m_pathToFile;
            return;
        }
        if(!m_xmlDoc.setContent(&xmlFile))
        {
            xmlFile.close();
            return;
        }
        else
        {
            loadXmlFileStatus->setText(tr("Статус файла: xml файл загружен, путь %1").arg(m_pathToFile));
            signXmlFileBtn->setEnabled(true);
        }
        xmlFile.close();
    }
}

void OceanXmlExporter::slotEditParntners()
{
    this->close();
    partnersWindow->show();
    connect(partnersWindow, &PartnersEditWindow::sigShowMainWindow, this, &OceanXmlExporter::slotShowWindow);
}

void OceanXmlExporter::slotShowWindow()
{
    this->show();
    getFirmsListData();
}

void OceanXmlExporter::slotSetFirmSignature()
{
    QVector<FirmData> firmVec = partnersWindow->getFirmListData();
    if(!firmVec.isEmpty())
    {
        for(int i(0); i < firmVec.size(); ++i)
        {
            if(firmVec.at(i).FirmName == firmsSelectorComboBox->currentText())
            {
                QDomElement docElem = m_xmlDoc.documentElement();
                QDomNode node = docElem.firstChild();
                while(!node.isNull())
                {
                    QDomElement nodeElement_1 = node.toElement();
                    if(!nodeElement_1.isNull())
                    {
                        if (nodeElement_1.tagName() == "ORG")
                        {
                            QDomNode childNode_1 = nodeElement_1.firstChild();
                            while(!childNode_1.isNull())
                            {
                                QDomElement nodeElement_2 = childNode_1.toElement();
                                if(nodeElement_2.tagName() == "CARD")
                                {
                                    QDomNode childNode_2 = nodeElement_2.firstChild();
                                    while(!childNode_2.isNull())
                                    {
                                        QDomElement nodeElement_3 = childNode_2.toElement();
                                        if(nodeElement_3.tagName() == "DOCUMENT")
                                        {
                                            QDomNode  childNode_3 = nodeElement_3.firstChild();
                                            while(!childNode_3.isNull())
                                            {
                                                QDomElement nodeElement_4 = childNode_3.toElement();
                                                if(nodeElement_4.tagName() == "ROW") //ROWS ELEMENTS
                                                {
                                                    if(nodeElement_4.attribute("NAME") == "FIRM_NAME"
                                                            || nodeElement_4.attribute("NAME") == "N10"
                                                            || nodeElement_4.attribute("NAME") == "FIRM_INN"
                                                            || nodeElement_4.attribute("NAME") == "FIRM_EDRPOU"
                                                            || nodeElement_4.attribute("NAME") == "INN")
                                                    {
                                                        QString newDataValue;
                                                        if(nodeElement_4.attribute("NAME") == "FIRM_NAME")
                                                        {
                                                            newDataValue = firmVec.at(i).FirmName;
                                                        }
                                                        else if(nodeElement_4.attribute("NAME") == "N10")
                                                        {
                                                            newDataValue = firmVec.at(i).FirmName;
                                                        }
                                                        else if(nodeElement_4.attribute("NAME") == "FIRM_INN")
                                                        {
                                                            newDataValue = firmVec.at(i).FirmInn;
                                                        }
                                                        else if(nodeElement_4.attribute("NAME") == "FIRM_EDRPOU")
                                                        {
                                                            newDataValue = firmVec.at(i).FirmOkpo;
                                                        }
                                                        else if(nodeElement_4.attribute("NAME") == "INN")
                                                        {
                                                            newDataValue = firmVec.at(i).FirmInn;
                                                        }
                                                        QDomNode  childNode_4 = nodeElement_4.firstChild();
                                                        while(!childNode_4.isNull())
                                                        {
                                                            QDomElement nodeElement_5 = childNode_4.toElement();
                                                            if(nodeElement_5.tagName() == "VALUE") //ROWS ELEMENTS
                                                            {
                                                                // New element;
                                                                qDebug() << "REPLACE";
                                                                QDomElement newValueTag = m_xmlDoc.createElement(QString("VALUE"));
                                                                QDomText newNodeText = m_xmlDoc.createTextNode(newDataValue);
                                                                newValueTag.appendChild(newNodeText);
                                                                nodeElement_4.replaceChild(newValueTag, nodeElement_5);
                                                            }
                                                            childNode_4 = childNode_4.nextSibling();
                                                        }
                                                    }
                                                }
                                                childNode_3 = childNode_3.nextSibling();
                                            }
                                        }
                                        childNode_2 = childNode_2.nextSibling();
                                    }
                                }
                                childNode_1 = childNode_1.nextSibling();
                            }
                        }
                    }
                    node = node.nextSibling();
                }
                QMessageBox::warning(NULL, "Документ подписан!", QString("Предпирятие %1 использованно для подписи").arg(firmVec.at(i).FirmName));
            }
        }
        QFile xmlFile(m_pathToFile);
        xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

        QTextStream ts(&xmlFile);
        //ts.setCodec("utf-8");
        ts << m_xmlDoc.toString();
        xmlFile.close();
    }
}

OceanXmlExporter::~OceanXmlExporter()
{

}
