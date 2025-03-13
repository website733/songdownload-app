#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QProcess>

class DownloaderApp : public QWidget {
    Q_OBJECT

public:
    DownloaderApp(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        urlInput = new QLineEdit(this);
        downloadButton = new QPushButton("Télécharger", this);
        outputLog = new QTextEdit(this);
        outputLog->setReadOnly(true);

        layout->addWidget(urlInput);
        layout->addWidget(downloadButton);
        layout->addWidget(outputLog);
        
        connect(downloadButton, &QPushButton::clicked, this, &DownloaderApp::startDownload);
    }

private slots:
    void startDownload() {
        QString url = urlInput->text();
        if (url.isEmpty()) {
            outputLog->append("Veuillez entrer une URL valide.");
            return;
        }
        
        QProcess *process = new QProcess(this);
        connect(process, &QProcess::readyReadStandardOutput, [this, process]() {
            outputLog->append(process->readAllStandardOutput());
        });
        connect(process, &QProcess::readyReadStandardError, [this, process]() {
            outputLog->append(process->readAllStandardError());
        });
        
        QString command = "yt-dlp -x --audio-format mp3 " + url;
        process->start(command);
    }

private:
    QLineEdit *urlInput;
    QPushButton *downloadButton;
    QTextEdit *outputLog;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    DownloaderApp window;
    window.setWindowTitle("Music Downloader");
    window.resize(400, 300);
    window.show();
    return app.exec();
}
