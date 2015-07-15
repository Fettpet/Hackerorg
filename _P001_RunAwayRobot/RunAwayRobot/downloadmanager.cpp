#include "downloadmanager.h"
#include <fstream>
#include <QFile>
#include <zlib.h>
#include <QDebug>
#include <fstream>
#include <QMessageBox>

void DownloadManager::SaveSourceToData(const char* filename){
    QString BildName;
    BildName = filename;

    if((BildName.contains(".png")) || (BildName.contains(".PNG"))){
        QImage Bild;
        QMessageBox t;
        Bild.loadFromData(*Source);
        t.setText(Bild.textKeys().value(0));
        t.exec();
        Bild.save(filename,"PNG");
    } else if((BildName.contains(".bmp")) || (BildName.contains(".BMP"))){
        QImage Bild;
        Bild.loadFromData(*Source);
        Bild.save(filename,"BMP");
    } else if((BildName.contains(".jpg")) || (BildName.contains(".JPG"))){
        QImage Bild;
        Bild.loadFromData(*Source);
        Bild.save(filename,"JPG");
    } else {
        QMessageBox t;
        t.setText("Speichern");
        t.exec();
        std::fstream datei;
        datei.open(filename, std::ios::out);
        datei << "hi";
        datei << (*Source).data();
        datei.close();
    }
}

DownloadManager::DownloadManager()
{
    manager = new QNetworkAccessManager();
    Source = new QByteArray();
    ausg.open("/home/fettpet/header.txt");
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
    finish = true;
}

QString DownloadManager::getCurrentHost(){
    return CurrentHost;
}

QString DownloadManager::getCookie(QUrl url, QString was){
    if(Cookie.keys().contains(url.host())){
        QString buffer = Cookie.take(url.host());
        if(buffer.contains(was)){
            buffer = buffer.mid(buffer.indexOf(was));
            buffer = buffer.mid(buffer.indexOf("=")+1);
            return buffer.left(buffer.indexOf(";"));
        } else{
            return "";
        }
    }else {
        return "";
    }
}
QString DownloadManager::getCookie(QUrl url){
    if(Cookie.keys().contains(url.host())){
        return Cookie.take(url.host());
    }else {
        return "";
    }
}
void DownloadManager::setCookie(QString Host, QString t){
    if(!Cookie.keys().contains(Host)){
        Cookie.insert(Host,t);
        return;
    }
    QString CurrentCookie = Cookie.take(Host);
    while(!t.isEmpty()){
        QString buffer = getNext(t);
        QString key= buffer.left(buffer.indexOf("="));
        QString value = buffer.mid(buffer.indexOf("=")+1);
        if(CurrentCookie.contains(key)){
            buffer = CurrentCookie.left(CurrentCookie.indexOf(key)) ;
            buffer += key + "=" + value + " ;";
            CurrentCookie = CurrentCookie.mid(CurrentCookie.indexOf(key));
            CurrentCookie = CurrentCookie.mid(CurrentCookie.indexOf(";")+1);
            CurrentCookie = buffer + CurrentCookie;
        } else {
            CurrentCookie += key + "=" + value + " ;";
        }

    };
    Cookie.insert(Host, CurrentCookie);
}

QString DownloadManager::getNext(QString &t){
    QString buffer;
    buffer = t.left(t.indexOf(";"));
    t = t.mid(t.indexOf(";")+2);
    return buffer;
}
QString DownloadManager::getValue(QString &t, QString was){
    if(t.contains(was)){
        QString buffer;
        buffer = t.mid(t.indexOf(was));
        buffer = buffer.mid(buffer.indexOf("=")+1);
        buffer = buffer.left(buffer.indexOf(";"));
        return buffer;
    }
    return "";
}

QUrl DownloadManager::getCurrentUrl(){
    return CurrentUrl;
}

void DownloadManager::doDownload(const QUrl &url, bool loeschen, const QByteArray Daten)
{
    if((url.isValid())){
        CurrentHost = url.host();
        CurrentUrl = url;
    } else{
        return;
    }
    finish = false;
    QString buffer, bufferausg;
    bufferausg = buffer = url.toString();
    if(!buffer.contains("http")){
        buffer = "http://" + buffer;
    }
 //   http://de84.die-staemme.de/game.php?village=39372&amp;screen=place&amp;action=command&amp;h=5ddf
    buffer = buffer.replace("%3F","?");
    buffer = buffer.replace("%3D","=");
    buffer = buffer.replace("%26","&");
    buffer = buffer.replace("&amp;","&");
    const QUrl back(buffer);
    QNetworkRequest request(back);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:14.0) Gecko/20100101 Firefox/14.0.1");
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Language" ,"de-DE,de;q=0.8,en-US;q=0.5,en;q=0.3");
 //   request.setRawHeader("Accept-Encoding", "gzip, deflate");
    if(Cookie.keys().contains(CurrentHost)){
        ausg << "Cookie: " << Cookie.take(CurrentHost).toStdString() << std::endl;
        request.setRawHeader("Cookie", Cookie.take(CurrentHost).toStdString().c_str());
    }
    request.setRawHeader("DNT", "1");
    request.setRawHeader("Connection", "keep-alive");

    QNetworkReply *reply;
    if( Daten.size() == 0){
        reply = manager->get(request);
    }else {
       reply = manager->post(request, Daten);
     }
     currentDownloads.append(reply);
     Loeschen.insert(reply, loeschen);

}

void DownloadManager::doDownload(const QString &url, bool loeschen, const QByteArray Daten){
    doDownload(QUrl(url), loeschen, Daten);
}

QStringList DownloadManager::getSRC(){
    QString buffer, bufferjava;
    QStringList href;
    buffer = Source->constData();

    while(buffer.contains("script type=\"text/javascript\"")){
        bufferjava = buffer.left(buffer.indexOf("<script type=\"text/javascript\""));
        bufferjava += buffer.mid(buffer.indexOf("</script>")+5);
        buffer = bufferjava;
    }

    while(buffer.contains("src=")){

        buffer = buffer.mid(buffer.indexOf("src=")+5);
        QString tohref = buffer.left(buffer.indexOf("\""));
        tohref = tohref.replace("&amp;", "&");
        if(tohref.contains("://")){
            href.append(tohref);
        }else {

            if(tohref[0] == QChar('/')){
                tohref = "http://" + getCurrentHost() +tohref;
            } else {
                tohref = "http://" + getCurrentHost() +"/"+tohref;
            }
            href.append(tohref);
        }
    }
    return href;
}

QStringList DownloadManager::getHREF(){
    QString buffer;
    QStringList src;
    buffer = Source->constData();
    while(buffer.contains("href=")){
        buffer = buffer.mid(buffer.indexOf("href="));
        buffer = buffer.mid(buffer.indexOf("\"")+1);
        QString tohref = buffer.left(buffer.indexOf("\""));
        tohref = tohref.replace("&amp;", "&");
        if(tohref.contains("://")){
            src.append(tohref);
        } else {
            if(tohref[0] == QChar('/')){
                tohref = "http://" + getCurrentHost() +tohref;
            } else {
                tohref = "http://" + getCurrentHost() +"/"+tohref;
            }
            src.append(tohref);
        }
    }
    return src;
}
QStringList DownloadManager::getHREF(QString contains){
    QStringList result, list;
    list = getHREF();
    for(int i=0; i<list.size();++i){
        if(list.value(i).contains(contains)){
            result.push_back(list.value(i));
        }
    }
    return result;
}
QStringList DownloadManager::getHREF(const char* contains){
    QStringList result, list;
    list = getHREF();
    for(int i=0; i<list.size();++i){
        if(list.value(i).contains(contains)){
            result.push_back(list.value(i));
        }
    }
    return result;
}

QStringList DownloadManager::getSource(){
    QStringList test;
    test.append(Source->constData());
    return test;
}
bool DownloadManager::isFinished(){
    return finish;
}
DownloadManager::~DownloadManager(){
    ausg.close();
}
void DownloadManager::ClearSource(){
    Source->clear();
}

void DownloadManager::downloadFinished(QNetworkReply *reply){
    QUrl url = reply->url();


    if(reply->rawHeader("Set-Cookie") != ""){
        QString buffer;
        buffer = reply->rawHeader("Set-Cookie");
        ausg << "Set-Cookie: " << QString(reply->rawHeader("Set-Cookie")).toStdString() << std::endl;
        ausg << "Host" << reply->url().toString().toStdString() << std::endl;
        setCookie(reply->url().host(), buffer);
    }
    if(reply->rawHeader("Location") != ""){
        QString buffer;
        buffer = reply->rawHeader("Location");
        if(!buffer.contains("://")){
            if(buffer[0] == QChar('/')){
                buffer = "http://" + CurrentHost +buffer;
            } else {
                buffer = "http://"+ CurrentHost+ "/" +buffer;
            }
        }
        emit Ausgabe("Location");
        emit Ausgabe(buffer);
        doDownload(QUrl(buffer));
        currentDownloads.removeAll(reply);
        reply->deleteLater();
        return;
    }

    QString Kompriemirungsalgo = reply->rawHeader("Content-Encoding");
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if(Loeschen.take(reply)) Source->clear();
        if(Kompriemirungsalgo != "gzip"){
            Source->append(reply->readAll());
        } else if (Kompriemirungsalgo == "gzip"){
            Source->append(gzipDecompress(reply->readAll()));
        } else{
            Source->append(reply->readAll());
        }
    }
    currentDownloads.removeAll(reply);
    reply->deleteLater();
    if (currentDownloads.isEmpty()){
         finish = true;
         emit finished();
    }
}

// Komprimierung
QByteArray DownloadManager::gzipDecompress( QByteArray compressData )
{
   //strip header
   compressData.remove(0, 10);

   const long int buffer_size = 32768*16;
   quint8 buffer[buffer_size];

   z_stream cmpr_stream;
   cmpr_stream.next_in = (unsigned char *)compressData.data();
   cmpr_stream.avail_in = compressData.size();
   cmpr_stream.total_in = 0;

 //  cmpr_stream.next_out = buffer;
//   cmpr_stream.avail_out = buffer_size;
   cmpr_stream.total_out = 0;

   cmpr_stream.zalloc = Z_NULL;
   cmpr_stream.zfree = Z_NULL;
   cmpr_stream.opaque = Z_NULL;

   int status = inflateInit2( &cmpr_stream, -8 );
   if (status != Z_OK) {
       qDebug() << "cmpr_stream error!";
   }

   QByteArray uncompressed;
   do {
       cmpr_stream.next_out = buffer;
       cmpr_stream.avail_out = buffer_size;

       status = inflate( &cmpr_stream, Z_NO_FLUSH );

       if (status == Z_OK || status == Z_STREAM_END)
       {
           QByteArray chunk = QByteArray::fromRawData((char *)buffer, buffer_size - cmpr_stream.avail_out);
           uncompressed.append( chunk );
       }
       else
       {
           inflateEnd(&cmpr_stream);
           break;
       }

   }
   while (cmpr_stream.avail_out == 0);

   return uncompressed;
}

/******************************************************************************************
                                Debugging
*******************************************************************************************/

void DownloadManager::LoadSourceFromData(const char* filename) {
    std::fstream datei;
    datei.open(filename, std::ios::in);
    std::string buffer;
    while(!datei.eof()){
        datei >> buffer;
        Source->append(buffer.c_str());
    }
}
