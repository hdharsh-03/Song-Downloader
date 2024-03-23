#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <string>

class Downloader : public QWidget {
Q_OBJECT

public:
    explicit Downloader(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);

        // URL input box
        urlLineEdit = new QLineEdit(this);
        urlLineEdit->setPlaceholderText("Enter URL");
        layout->addWidget(urlLineEdit);

        // File name input box
        fileNameLineEdit = new QLineEdit(this);
        fileNameLineEdit->setPlaceholderText("Exported File Name");
        layout->addWidget(fileNameLineEdit);

        // Location selection button
        locationButton = new QPushButton("Select Location", this);
        layout->addWidget(locationButton);

        // Download button
        downloadButton = new QPushButton("Download File", this);
        layout->addWidget(downloadButton);

        setLayout(layout);

        connect(locationButton, &QPushButton::clicked, this, &Downloader::selectLocation);
        connect(downloadButton, &QPushButton::clicked, this, &Downloader::downloadFile);
    }

private slots:
    void selectLocation() {
        QString selectedFile = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath());
        if (!selectedFile.isEmpty()) {
            fileNameLineEdit->setText(selectedFile);
        }
    }

    void downloadFile() {
        QString url = urlLineEdit->text();
        QString outputFileName = fileNameLineEdit->text();

        if (url.isEmpty() || outputFileName.isEmpty()) {
            QMessageBox::warning(this, "Warning", "URL or output file name cannot be empty.");
            return;
        }

        CURL *curl;
        FILE *fp;
        CURLcode res;

        std::string urlStr = url.toStdString();
        std::string outputFileNameStr = outputFileName.toStdString();

        curl = curl_easy_init();
        if (curl) {
            fp = fopen(outputFileNameStr.c_str(), "wb");
            if (fp == nullptr) {
                QMessageBox::critical(this, "Error", "Failed to open file for writing.");
                return;
            }

            curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                QMessageBox::critical(this, "Error", "Failed to download file.");
            } else {
                QMessageBox::information(this, "Success", "File downloaded successfully.");
            }

            curl_easy_cleanup(curl);
            fclose(fp);
        } else {
            QMessageBox::critical(this, "Error", "Failed to initialize curl.");
        }
    }

private:
    static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        return fwrite(ptr, size, nmemb, stream);
    }

    QLineEdit *urlLineEdit;
    QLineEdit *fileNameLineEdit;
    QPushButton *locationButton;
    QPushButton *downloadButton;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Downloader downloader;
    downloader.setWindowTitle("File Downloader");
    downloader.resize(300, 150);
    downloader.show();
    return QApplication::exec();
}

#include "main.moc"
