
#include <QtWidgets/qmainwindow.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qcombobox.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_translateButton_clicked();
    void on_networkReply_finished(QNetworkReply* reply);

private:
    QLineEdit *wordLineEdit;
    QPushButton *translateButton;
    QLabel *translationLabel;
    QNetworkAccessManager *networkManager;
    QComboBox *selectLang;
};