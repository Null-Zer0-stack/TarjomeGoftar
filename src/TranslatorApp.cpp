#include "TranslatorApp.h"
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qwidget.h>
#include <QtCore/qurlquery.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonvalue.h>
#include <QtCore/qjsonarray.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{


        // Set the initial size of the window
        resize(400, 400);
        
        // Initialize UI widgets
        wordLineEdit = new QLineEdit(this);
        translateButton = new QPushButton("ترجمه کن", this);

        translationLabel = new QLabel("ترجمه اینجا پدیدار می شود", this);
        translationLabel->setWordWrap(true); // Allow the label to wrap long text

        selectLang = new QComboBox(this);
        selectLang->setPlaceholderText("زبان مورد نظر را انتخاب کنید");
        selectLang->addItem("انگلیسی به فارسی");
        selectLang->addItem("فارسی به انگلیسی");
        

        // Create a layout and add widgets
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(wordLineEdit);
        layout->addWidget(translateButton);
        layout->addWidget(translationLabel);
        layout->addWidget(selectLang);

        // Create a central widget to hold the layout
        QWidget* centralWidget = new QWidget(this);
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);

        // Initialize the network manager
        networkManager = new QNetworkAccessManager(this);

        // Connect signals and slots
        connect(translateButton, &QPushButton::clicked, this, &MainWindow::on_translateButton_clicked);
        connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::on_networkReply_finished);
    }

    MainWindow::~MainWindow()
    {
        delete networkManager;
    }

    void MainWindow::on_translateButton_clicked()
    {        
        QString wordToTranslate = wordLineEdit->text().trimmed(); // trim whitespace
        if (wordToTranslate.isEmpty()) {
            translationLabel->setText("لطفا کلمه مورد نظر خود را وارد کنید");
            return;
        }

        // 2. Determine the language direction based on the selected item text
        QString selectedText = selectLang->currentText();
        QString sourceLangCode;
        QString targetLangCode;

        // Check which item is currently selected
        if (selectedText == "انگلیسی به فارسی") {
            sourceLangCode = "en";
            targetLangCode = "fa";
        }
        else if (selectedText == "فارسی به انگلیسی") {
            sourceLangCode = "fa";
            targetLangCode = "en";
        }
        else {
            // Handle case where no selection is made (optional)
            translationLabel->setText("لطفا جهت ترجمه را انتخاب کنید.");
            return;
        }

        // 3. Construct the API request URL
        QUrl url("https://translate.googleapis.com/translate_a/single");
        QUrlQuery query;
        query.addQueryItem("client", "gtx");

        // Use the determined language codes
        query.addQueryItem("sl", sourceLangCode); // Source language
        query.addQueryItem("tl", targetLangCode); // Target language

        query.addQueryItem("dt", "t");   // Include translation text
        query.addQueryItem("dt", "bd");  // Include dictionary data (multiple meanings)
        query.addQueryItem("q", wordToTranslate); // Word to translate
        url.setQuery(query);

        // 4. Send the request
        QNetworkRequest request(url);
        networkManager->get(request);

        translationLabel->setText("درحال ترجمه...");
    }

    void MainWindow::on_networkReply_finished(QNetworkReply * reply)
    {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();

            // Google Translate API returns a JSON array, not an object.
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonArray rootArray = jsonResponse.array();

            QString fullTranslation;
            if (rootArray.count() > 0) {
                // Get the main translation
                QJsonArray translationArray = rootArray[0].toArray();
                if (translationArray.count() > 0) {
                    fullTranslation += translationArray[0].toArray()[0].toString();
                }

                // Get the additional meanings from the dictionary data (index 1)
                if (rootArray.count() > 1 && rootArray[1].isArray()) {
                    QJsonArray dictionaryArray = rootArray[1].toArray();
                    if (dictionaryArray.count() > 0) {
                        fullTranslation += "\n\n**معانی دیگر:**";

                        for (const QJsonValue& partOfSpeechValue : dictionaryArray) {
                            QJsonArray partOfSpeechArray = partOfSpeechValue.toArray();
                            if (partOfSpeechArray.count() > 0) {
                                QString partOfSpeech = partOfSpeechArray[0].toString();
                                fullTranslation += "\n" + partOfSpeech;

                                if (partOfSpeechArray.count() > 1 && partOfSpeechArray[1].isArray()) {
                                    QJsonArray meaningArray = partOfSpeechArray[1].toArray();
                                    for (const QJsonValue& meaningValue : meaningArray) {
                                        fullTranslation += ", " + meaningValue.toString();
                                    }
                                }
                            }
                        }
                    }
                }

                translationLabel->setText(fullTranslation);
            }
            else {
                translationLabel->setText("ترجمه پیدا نشد");
            }
        }
        else {
            translationLabel->setText("ارور: " + reply->errorString());
        }

        reply->deleteLater();
    }