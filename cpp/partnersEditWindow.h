#ifndef PARNTNERSEDITWINDOW_H
#define PARNTNERSEDITWINDOW_H

#include <QDialog>
#include "mainWindow.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QGroupBox;
class QComboBox;
class QDomDocument;
class QLineEdit;
class QGridLayout;
class XmlParser;

class PartnersEditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PartnersEditWindow(QWidget *parent = 0);
    QVector<FirmData> getFirmListData();

    ~PartnersEditWindow();

signals:
    void sigShowMainWindow();
    //void sigSendFirmsList(QStringList);

protected:
    void closeEvent(QCloseEvent * event);
    void resizeEvent(QResizeEvent* event);

public slots:
    void slotAddPartner();
    void slotAddFirmNames(QStringList);
    void slotUpdateFirmData();
    void slotLoadCurrentFirmData(QString firmName);
    void slotDeleteCurrentFirm();

private:
    QGridLayout *mainLayout;

    QLineEdit   *FirmNameLe;
    QLineEdit   *FirmOkpoLe;
    QLineEdit   *FirmInnLe;
    QLineEdit   *FirmDateInnLe;
    QLineEdit   *FirmAddressLe;
    QLineEdit   *FirmTelephoneLe;
    QLineEdit   *FirmBankLe;
    QLineEdit   *FirmBankAccountLe;
    QLineEdit   *FirmCertificateLe;

    QLabel      *FirmNameLb;
    QLabel      *FirmOkpoLb;
    QLabel      *FirmInnLb;
    QLabel      *FirmDateInnLb;
    QLabel      *FirmAddressLb;
    QLabel      *FirmTelephoneLb;
    QLabel      *FirmBankLb;
    QLabel      *FirmBankAccountLb;
    QLabel      *FirmCertificateLb;

    QPushButton *SaveBtn;
    QPushButton *CancelBtn;
    QPushButton *UpdateFirmBtn;
    QPushButton *DeleteFirmBtn;

    QComboBox   *FirmsList;

    // Xml parser;
    XmlParser           *xmlParser;
    FirmData            m_DataStruct;
};

#endif // PARNTNERSEDITWINDOW_H
