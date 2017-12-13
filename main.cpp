#include <QCoreApplication>
#include <QDateTime>
#include <QTimer>
#include "controller/worker.h"

#define VERSION "0.1 beta"

QString processArgs(int argc, char **argv)
{
    bool err = true;
    QString url;
    if (argc < 2) {
        printf(" -aid / --access-id ACCESS_ID\t: Access ID for SkyWave API\n\n"
               " -e / --end-utc END_UTC\t\t: UTC time for end data (yyyy-MM-dd#HH:mm:ss)\n\n"
               " -g / --gateway GATEWAY\t\t: SkyWave API Gateway address\n\n"
               " -h / --help\t\t\t: Print this help info\n\n"
               " -id / --module-id\t\t: SkyWave Module ID\n\n"
               " -pwd / --password PASSWORD\t: Password for SkyWave API\n\n"
               " -s / --start-utc START_UTC\t: UTC time for start data (yyyy-MM-dd#HH:mm:ss)\n\n"
               " -sm / --sin-min SIN_MIN\t: SIN MIN Config (SIN#MIN)\n\n"
               " -t / --timeout TIMEOUT(ms)\t: Timeout for end process (default 5000ms)\n\n"
               " -v / --version\t\t\t: Print version of plugins\n\n"
               " ex 1 : -g http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/ -aid 150103286 -pwd ZRM3B9SSDI -sm 128#1 -s 2017-03-27#03:43:02 -t 10000\n\n"
               " ex 2 : -g http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/ -aid 150103286 -pwd ZRM3B9SSDI -sm 128#1 -s 2017-03-27#03:43:02 -e 2017-03-27#04:43:02 -t 10000\n\n"
               " ex 3 : -g http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/ -aid 150103286 -pwd ZRM3B9SSDI -sm 128#1 -id 01020268SKY7559 -s 2017-03-27#03:43:02 -e 2017-03-27#04:43:02 -t 10000\n\n"
              );
        err = false;
    } else if (argc == 2) {
        QString arg1(argv[1]);
        if (arg1 == QLatin1String("-h") ||
            arg1 == QLatin1String("--help")) {
            printf(" -aid / --access-id ACCESS_ID\t: Access ID for SkyWave API\n\n"
                   " -e / --end-utc END_UTC\t\t: UTC time for end data (yyyy-MM-dd#HH:mm:ss)\n\n"
                   " -g / --gateway GATEWAY\t\t: SkyWave API Gateway address\n\n"
                   " -h / --help\t\t\t: Print this help info\n\n"
                   " -id / --module-id\t\t: SkyWave Module ID\n\n"
                   " -pwd / --password PASSWORD\t: Password for SkyWave API\n\n"
                   " -s / --start-utc START_UTC\t: UTC time for start data (yyyy-MM-dd#HH:mm:ss)\n\n"
                   " -sm / --sin-min SIN_MIN\t: SIN MIN Config (SIN#MIN)\n\n"
                   " -t / --timeout TIMEOUT(ms)\t: Timeout for end process (default 5000ms)\n\n"
                   " -v / --version\t\t\t: Print version of plugins\n\n"
                   " ex 1 : -g http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/ -aid 150103286 -pwd ZRM3B9SSDI -sm 128#1 -s 2017-03-27#03:43:02 -t 10000\n\n"
                   " ex 2 : -g http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/ -aid 150103286 -pwd ZRM3B9SSDI -sm 128#1 -s 2017-03-27#03:43:02 -e 2017-03-27#04:43:02 -t 10000\n\n"
                   " ex 3 : -g http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/ -aid 150103286 -pwd ZRM3B9SSDI -sm 128#1 -id 01020268SKY7559 -s 2017-03-27#03:43:02 -e 2017-03-27#04:43:02 -t 10000\n\n"
                  );
            err = false;
        } else if (arg1 == QLatin1String("-v") ||
                 arg1 == QLatin1String("--version")) {
            QString compilationTime = QString("%1 %2").arg(__DATE__).arg(__TIME__);
            QString version = VERSION;
            printf("Plugin SkyWave Version:  %s\nPT. DaunBiru Engineering\nwww.daunbiru.com\n\n"
                   "build on: %s (UTC+7)\n",
                   version.toUtf8().data(),
                   compilationTime.toUtf8().data());
            err = false;
        }
    }
    else  if (argc > 2) {
        QString gateway = "",
                module_id = "",
                access_id = "",
                password = "",
//                sin_min = "",
                start_utc = "",
                end_utc = "";
//        int timeout = 0;
        for (int i = 1; i < argc; i++) {
            QString arg1(argv[i]);
            if (arg1 == QLatin1String("-aid") ||
                arg1 == QLatin1String("--access-id")) {
                access_id = argv[i+1];
//                qDebug() << argv[i+1];
            } else if (arg1 == QLatin1String("-e") ||
                       arg1 == QLatin1String("--end-utc")) {
                end_utc = argv[i+1];
                end_utc.replace("#", " ");
//                qDebug() << argv[i+1];
            } else if (arg1 == QLatin1String("-g") ||
                       arg1 == QLatin1String("--gateway")) {
                gateway = argv[i+1];
//                qDebug() << argv[i+1];
            } else if (arg1 == QLatin1String("-id") ||
                       arg1 == QLatin1String("--module-id")) {
                module_id = argv[i+1];
//                qDebug() << argv[i+1];
            } else if (arg1 == QLatin1String("-pwd") ||
                       arg1 == QLatin1String("--password")) {
                password = argv[i+1];
//                qDebug() << argv[i+1];
            } else if (arg1 == QLatin1String("-s") ||
                       arg1 == QLatin1String("--start-utc")) {
                start_utc = argv[i+1];
                start_utc.replace("#", " ");
//                qDebug() << argv[i+1];
            } /*else if (arg1 == QLatin1String("-t") ||
                       arg1 == QLatin1String("--timeout")) {
                timeout = atoi(argv[i+1]);
            }*/
        }
//        qDebug() << "gateway = " + gateway;
//        qDebug() << "access id = " + access_id;
//        qDebug() << "password = " + password;
//        qDebug() << "module id = " + module_id;
//        qDebug() << "start utc = " + start_utc;
//        qDebug() << "end utc = " + end_utc;
//        gateway = "http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/";
//        access_id = "150103286";
//        password = "ZRM3B9SSDI";
//        start_utc = "2017-11-21 00:00:00";
        if (!gateway.isEmpty()) {
            if (!access_id.isEmpty() && !password.isEmpty()) {
                if (QDateTime::fromString(start_utc, "yyyy-MM-dd HH:mm:ss").isValid()) {
//                    if (timeout <= 0) timeout = 5000;
//                     "http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/?access_id=150103286&password=ZRM3B9SSDI&start_utc=2016-09-02%2005:40:05"
//                     "http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/?access_id=150103286&password=ZRM3B9SSDI&start_utc=2017-03-27 03:43:02&end_utc=2017-03-27 04:43:02&mobile_id=01020268SKY7559"
                    if (QDateTime::fromString(end_utc, "yyyy-MM-dd HH:mm:ss").isValid()) {
                        if (!module_id.isEmpty()) {
                            url = gateway + "?access_id=" + access_id + "&password=" + password + "&start_utc=" + start_utc + "&end_utc=" + end_utc + "&mobile_id=" + module_id;
                        } else {
                            url = gateway + "?access_id=" + access_id + "&password=" + password + "&start_utc=" + start_utc + "&end_utc=" + end_utc;
                        }
                    } else {
                        if (!module_id.isEmpty()) {
                            url = gateway + "?access_id=" + access_id + "&password=" + password + "&start_utc=" + start_utc + "&mobile_id=" + module_id;
                        } else {
                            url = gateway + "?access_id=" + access_id + "&password=" + password + "&start_utc=" + start_utc;
                        }
                    }
                    err = false;
                }
            } else {
                err = true;
            }
        } else {
            err = true;
        }
    }
    if (err) {
        printf("{\"ERR\": \"Wrong Pluggins Commands\"}\n\n");
        return "";
    }

    return url;
}

int timeOut(int argc, char **argv) {
    int tOut = 60000;

    if (argc > 2) {
        for (int i = 1; i < argc; i++) {
            QString arg1(argv[i]);
            if (arg1 == QLatin1String("-t") || arg1 == QLatin1String("--timeout")) {
                tOut = atoi(argv[i+1]);
            }
        }
    }

//    qDebug() << "timeout = " + QString::number(tOut);

    return tOut;
}

QString SinMin(int argc, char **argv) {
    QString sin_min = "128#1";

    if (argc > 2) {
        for (int i = 1; i < argc; i++) {
            QString arg1(argv[i]);
            if (arg1 == QLatin1String("-sm") || arg1 == QLatin1String("--sin-min")) {
                sin_min = argv[i+1];
            }
        }
    }

//    qDebug() << "timeout = " + QString::number(tOut);

    return sin_min;
}

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    worker w;
    a.connect(&w, SIGNAL(close()), &a, SLOT(quit()));
    QString url = processArgs(argc, argv);
    QString sinMin = SinMin(argc, argv);
    if (!url.isEmpty()) {
        w.request(url, sinMin);
        QTimer::singleShot(timeOut(argc, argv), &a, SLOT(quit()));
    //    QTimer::singleShot(10000, &a, SLOT(quit()));

        return a.exec();
    } else {
        return 0;
    }
}
