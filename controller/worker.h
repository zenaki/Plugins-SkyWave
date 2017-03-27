#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTimer>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#define BYTE_TO_BINARY(byte) \
    (byte & 0x80 ? '1' : '0'), \
    (byte & 0x40 ? '1' : '0'), \
    (byte & 0x20 ? '1' : '0'), \
    (byte & 0x10 ? '1' : '0'), \
    (byte & 0x08 ? '1' : '0'), \
    (byte & 0x04 ? '1' : '0'), \
    (byte & 0x02 ? '1' : '0'), \
    (byte & 0x01 ? '1' : '0')

class worker : public QObject
{
    Q_OBJECT
public:
    explicit worker(QObject *parent = 0);

    void request(QString urls);
    void parsing(QByteArray data);

    int process;

    QString format_5cut_32get(QString biner);
    QStringList parsingRawPayload(QString RawData);
private:
    QNetworkAccessManager *manager;

private slots:
    void replyFinished(QNetworkReply *reply);
    void timeOut();

signals:
    void close();
};

#endif // WORKER_H
