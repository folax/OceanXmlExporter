#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDomDocument>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QGroupBox;
class QString;
class QDomDocument;
class QComboBox;
class PartnersEditWindow;

struct FirmData
{
   QString FirmName;
   QString FirmOkpo;
   QString FirmInn;
   QString FirmDateInn;
   QString FirmAddress;
   QString FirmTelephone;
   QString FirmBank;
   QString FirmBankAccount;
   QString FirmCertificate;
};

class OceanXmlExporter : public QDialog
{
    Q_OBJECT

public:
    explicit OceanXmlExporter(QWidget *parent = 0);
    void getFirmsListData();
    ~OceanXmlExporter();

public slots:
    void slotOpenFile();
    void slotEditParntners();
    void slotShowWindow();
    void slotSetFirmSignature();

private:

    // Gui
    QHBoxLayout         *mainLayout;
    QVBoxLayout         *loadXmlLayout_1;
    QVBoxLayout         *loadXmlLayout_2;
    QGroupBox           *loadXmlGroup_1;
    QGroupBox           *loadXmlGroup_2;
    QPushButton         *loadXmlFileBtn;
    QPushButton         *checkXmlFileBtn;
    QLabel              *loadXmlFileStatus;
    QComboBox           *firmsSelectorComboBox;
    QPushButton         *editFirmsBtn;
    QPushButton         *signXmlFileBtn;
    QPushButton         *cancelBtn;

    // Path to file;
    QString             m_pathToFile;
    QDomDocument        m_xmlDoc;

    // Second gui window;
    PartnersEditWindow  *partnersWindow;
};

#endif // DIALOG_H
