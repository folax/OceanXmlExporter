#include "PartnersEditWindow.h"
#include "xmlparser.h"

#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>

PartnersEditWindow::PartnersEditWindow(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Window);
    setWindowTitle("Создание/редактирование предприятия");
    // setFixedHeight(305);
    resize(400, 305);

    mainLayout = new QGridLayout(this);

    FirmsList = new QComboBox;

    FirmNameLe =        new QLineEdit();
    FirmNameLe->setText(tr("ФОП Иванов А.Ю.(Пример заполнения)"));

    FirmOkpoLe =        new QLineEdit();
    FirmOkpoLe->setText(tr("2538420793"));

    FirmInnLe =         new QLineEdit();
    FirmInnLe->setText(tr("2538420793"));

    FirmDateInnLe =     new QLineEdit();
    FirmDateInnLe->setText(tr("21.03.2007"));

    FirmAddressLe =     new QLineEdit();
    FirmAddressLe->setText(tr("50000, Кривой Рог"));

    FirmTelephoneLe =   new QLineEdit();
    FirmTelephoneLe->setText(tr("0564716142"));

    FirmBankLe =        new QLineEdit();
    FirmBankLe->setText(tr("РАЙФФАЙЗЕН БАНК"));

    FirmBankAccountLe = new QLineEdit();
    FirmBankAccountLe->setText(tr("26001371810"));

    FirmCertificateLe = new QLineEdit();
    FirmCertificateLe->setText(tr("200036451"));

    FirmNameLb =        new QLabel(tr("Название фирмы:"));
    FirmOkpoLb =        new QLabel(tr("Код ОКПО:"));
    FirmInnLb =         new QLabel(tr("ИНН:"));
    FirmDateInnLb =     new QLabel(tr("Дата ИНН:"));
    FirmAddressLb =     new QLabel(tr("Адресс:"));
    FirmTelephoneLb =   new QLabel(tr("Телефон:"));
    FirmBankLb =        new QLabel(tr("Банк:"));
    FirmBankAccountLb = new QLabel(tr("Счёт:"));
    FirmCertificateLb = new QLabel(tr("Номер свидетельства:"));

    mainLayout->addWidget(FirmsList, 0, 0, 1, 2);

    mainLayout->addWidget(FirmNameLb, 1, 0);
    mainLayout->addWidget(FirmNameLe, 1, 1, 1, 2);

    mainLayout->addWidget(FirmOkpoLb, 2, 0);
    mainLayout->addWidget(FirmOkpoLe, 2, 1, 1, 2);

    mainLayout->addWidget(FirmInnLb, 3, 0);
    mainLayout->addWidget(FirmInnLe, 3, 1, 1, 2);

    mainLayout->addWidget(FirmDateInnLb, 4, 0);
    mainLayout->addWidget(FirmDateInnLe, 4, 1, 1, 2);

    mainLayout->addWidget(FirmAddressLb, 5, 0);
    mainLayout->addWidget(FirmAddressLe, 5, 1, 1, 2);

    mainLayout->addWidget(FirmTelephoneLb, 6, 0);
    mainLayout->addWidget(FirmTelephoneLe, 6, 1, 1, 2);

    mainLayout->addWidget(FirmBankLb, 7, 0);
    mainLayout->addWidget(FirmBankLe, 7, 1, 1, 2);

    mainLayout->addWidget(FirmBankAccountLb, 8, 0);
    mainLayout->addWidget(FirmBankAccountLe, 8, 1, 1, 2);

    mainLayout->addWidget(FirmCertificateLb, 9, 0);
    mainLayout->addWidget(FirmCertificateLe, 9, 1, 1, 2);

    SaveBtn =       new QPushButton(tr("Сохранить изменения"));
    UpdateFirmBtn = new QPushButton(tr("Обновить данные"));
    CancelBtn =     new QPushButton(tr("Отмена"));
    DeleteFirmBtn = new QPushButton(tr("Удалить фирму"));

    mainLayout->addWidget(DeleteFirmBtn, 0, 2, 1, 1);

    mainLayout->addWidget(SaveBtn,      10, 0);
    mainLayout->addWidget(UpdateFirmBtn,10, 1);
    mainLayout->addWidget(CancelBtn,    10, 2);

    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

    // Connect
    connect(SaveBtn,        &QPushButton::clicked, this, slotAddPartner);
    connect(CancelBtn,      &QPushButton::clicked, this, sigShowMainWindow);
    connect(CancelBtn,      &QPushButton::clicked, this, close);
    connect(DeleteFirmBtn,  &QPushButton::clicked, this, slotDeleteCurrentFirm);

    // Xml parser initialization;
    xmlParser = new XmlParser();
    connect(xmlParser,      &XmlParser::sigDataFromXml, this, slotAddFirmNames);
    connect(UpdateFirmBtn,  &QPushButton::clicked,      this, &PartnersEditWindow::slotUpdateFirmData);

    xmlParser->readAllDataFromXML();

    connect(FirmsList, SIGNAL(activated(QString)), SLOT(slotLoadCurrentFirmData(QString)));
}

QVector<FirmData> PartnersEditWindow::getFirmListData()
{
    return xmlParser->getFirmsData();
}

void PartnersEditWindow::closeEvent(QCloseEvent*)
{
    emit sigShowMainWindow();
}

void PartnersEditWindow::resizeEvent(QResizeEvent*)
{
    //qDebug() << this->height();
}

void PartnersEditWindow::slotAddPartner()
{
    m_DataStruct.FirmName           = FirmNameLe->text();
    m_DataStruct.FirmOkpo           = FirmOkpoLe->text();
    m_DataStruct.FirmInn            = FirmInnLe->text();
    m_DataStruct.FirmDateInn        = FirmDateInnLe->text();
    m_DataStruct.FirmAddress        = FirmAddressLe->text();
    m_DataStruct.FirmTelephone      = FirmTelephoneLe->text();
    m_DataStruct.FirmBank           = FirmBankLe->text();
    m_DataStruct.FirmBankAccount    = FirmBankAccountLe->text();
    m_DataStruct.FirmCertificate    = FirmCertificateLe->text();
    xmlParser->addItemToXML(m_DataStruct);
}

void PartnersEditWindow::slotAddFirmNames(QStringList data)
{
    //qDebug() << "PartnersEditWindow::slotAddFirmNames : " << data;
    FirmsList->clear();
    FirmsList->addItems(data);
}

void PartnersEditWindow::slotUpdateFirmData()
{
    FirmData elementsData;
    elementsData.FirmName           = FirmNameLe->text();
    elementsData.FirmOkpo           = FirmOkpoLe->text();
    elementsData.FirmInn            = FirmInnLe->text();
    elementsData.FirmDateInn        = FirmDateInnLe->text();
    elementsData.FirmAddress        = FirmAddressLe->text();
    elementsData.FirmTelephone      = FirmTelephoneLe->text();
    elementsData.FirmBank           = FirmBankLe->text();
    elementsData.FirmBankAccount    = FirmBankAccountLe->text();
    elementsData.FirmCertificate    = FirmCertificateLe->text();
    xmlParser->UpdateFirmData(FirmsList->currentText(), elementsData);
}

void PartnersEditWindow::slotLoadCurrentFirmData(QString firmName)
{
    FirmData data = xmlParser->getCurrentFirmData(firmName);
    FirmNameLe->setText(data.FirmName);
    FirmOkpoLe->setText(data.FirmOkpo);
    FirmInnLe->setText(data.FirmInn);
    FirmDateInnLe->setText(data.FirmDateInn);
    FirmAddressLe->setText(data.FirmAddress);
    FirmTelephoneLe->setText(data.FirmTelephone);
    FirmBankLe->setText(data.FirmBank);
    FirmBankAccountLe->setText(data.FirmBankAccount);
    FirmCertificateLe->setText(data.FirmCertificate);
}

void PartnersEditWindow::slotDeleteCurrentFirm()
{
    xmlParser->removeFirmFromXML(FirmsList->currentText());
}

PartnersEditWindow::~PartnersEditWindow()
{
    delete xmlParser;
}
