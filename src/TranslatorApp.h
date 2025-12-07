
#include <QtWidgets/qmainwindow.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qaction.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();


private slots:
    void on_translateButton_clicked();
    void on_networkReply_finished(QNetworkReply* reply);
    void applyLightTheme();
    void applyDarkTheme();
   
private:
    QLineEdit* wordLineEdit;
    QPushButton* translateButton;
    QLabel* translationLabel;
    QNetworkAccessManager* networkManager;
    QComboBox* selectLang;
    QMenu* themeMenu;
    QAction* lightTheme;
    QAction* darkTheme;
    void createThemeMenu();
};
