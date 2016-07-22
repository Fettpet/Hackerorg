#ifndef DOWNLOADMANAGER
#define DOWNLOADMANAGER

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QString>
#include <map>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QMap>
#include <QUrl>
#include <fstream>
#include <stdio.h>

class DownloadManager: public QObject
{
    Q_OBJECT
public:
    void ClearSource();
    QUrl getCurrentUrl();
    QString getCurrentHost();
    QByteArray gzipDecompress( QByteArray compressData );
    QStringList getSource();
    DownloadManager();
    ~DownloadManager();
    void doDownload(const QUrl &url, bool loeschen = false, const QByteArray Daten = "");
    void doDownload(const QString &url, bool loeschen = false, const QByteArray Daten = "");
    void SaveSourceToData(const char* filename);
    void LoadSourceFromData(const char* filename);
   // QString saveFileName(const QUrl &url);
    bool isFinished();
    QStringList getSRC();
    QStringList getHREF();
    QStringList getHREF(QString contains);
    QStringList getHREF(const char* contains);
    QString getCookie(QUrl, QString was);
    QString getCookie(QUrl);
public slots:
    void downloadFinished(QNetworkReply *reply);
signals:
    void finished();
    void Ausgabe(QString);
private:
    void setCookie(QString Host, QString t);
    QString getNext(QString &t);
    QString getValue(QString &t, QString was);

    std::ofstream ausg;
    QNetworkAccessManager *manager;
    QList<QNetworkReply *> currentDownloads;
    QMap<QNetworkReply*, bool> Loeschen;
    QMap<QString, QString> Cookie;
    bool finish;
    QByteArray *Source;
    QString CurrentHost;
    QUrl CurrentUrl;
};

#endif

