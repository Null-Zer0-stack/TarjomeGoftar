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
        translateButton = new QPushButton("Translate", this);
        translationLabel = new QLabel("Translation will appear here...", this);
        translationLabel->setWordWrap(true); // Allow the label to wrap long text

        // Create a layout and add widgets
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(wordLineEdit);
        layout->addWidget(translateButton);
        layout->addWidget(translationLabel);

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
        QString wordToTranslate = wordLineEdit->text();
        if (wordToTranslate.isEmpty()) {
            translationLabel->setText("Please enter a word.");
            return;
        }

        // Construct the URL for a more comprehensive translation API
        // Note: The structure of this API might change, so this is an example.
        QUrl url("https://translate.googleapis.com/translate_a/single");
        QUrlQuery query;
        query.addQueryItem("client", "gtx");
        query.addQueryItem("sl", "en"); // Source language (English)
        query.addQueryItem("tl", "fa"); // Target language (Persian)
        query.addQueryItem("dt", "t");  // Include translation text
        query.addQueryItem("dt", "bd"); // Include dictionary data (multiple meanings)
        query.addQueryItem("q", wordToTranslate); // Word to translate
        url.setQuery(query);

        QNetworkRequest request(url);
        networkManager->get(request);

        translationLabel->setText("Translating...");
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
                        fullTranslation += "\n\n**Additional Meanings:**";

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
                translationLabel->setText("Translation not found.");
            }
        }
        else {
            translationLabel->setText("Error: " + reply->errorString());
        }

        reply->deleteLater();
    }