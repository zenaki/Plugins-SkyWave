#include "worker.h"

worker::worker(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));

    process = 1;
}

void worker::request(QString urls, QString sinMin)
{
    QNetworkRequest request;
//    urls = "http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/?access_id=150103286&password=ZRM3B9SSDI&start_utc=2017-03-23%2000:00:00";
//    urls = "http://m2prime.aissat.com/RestMessages.svc/get_return_messages.json/?access_id=150103286&password=ZRM3B9SSDI&start_utc=2017-03-27 03:43:02&end_utc=2017-03-27 04:43:02&mobile_id=01020268SKY7559";
    QUrl url =  QUrl::fromEncoded(urls.toLocal8Bit().data());

//    qDebug() << url;
    sinMin_conf = sinMin;
    request.setUrl(url);
    manager->get(request);

//    QTimer *t = new QTimer(this);
//    connect(t, SIGNAL(timeout()), this, SLOT(timeOut()));
//    t->start(50000);
//    t->start(tOut);
}

void worker::parsing(QByteArray data)
{
//    monita_cfg.config = cfg.read("CONFIG");
//    bool ok;
    QString MessageUTC;
    QString RawBiner;
    QStringList PayLoadData;
//    bool getData = false;
    int index;
    char temp[16];
    QJsonDocument JsonDoc = QJsonDocument::fromJson(data);
    QJsonObject object = JsonDoc.object();
    MessageUTC = object.value("NextStartUTC").toString();

    QJsonArray array = object.value("Messages").toArray();

    if (!MessageUTC.isEmpty()) {
//        for (int i = 0; i < monita_cfg.sky[indexGateway].jml_modem; i++) {
//            monita_cfg.sky[indexGateway].mdm[i].val_tu.clear();
//            monita_cfg.sky[indexGateway].mdm[i].query.clear();
//        }
//        log.write("SkyWave","Mulai Parsing URL " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz"), monita_cfg.config.at(9).toInt());
        QJsonArray valArray;
        foreach (const QJsonValue & v, array) {
            QJsonObject valObject;
//            for (int i = 0; i < monita_cfg.sky[indexGateway].jml_modem; i++) {
//                if (v.toObject().value("MobileID").toString() == "01093397SKY2DA6") {
//                    int t;
//                    t++;
//                }
//                if (v.toObject().value("MobileID").toString() ==
//                        monita_cfg.sky[indexGateway].mdm[i].modem_id) {
                    QStringList SIN_MIN = sinMin_conf.split(","); QStringList SIN_MIN_List;
                    for (int j = 0; j < SIN_MIN.length(); j++) {SIN_MIN_List.append(SIN_MIN.at(j).split("#"));}
                    for (int j = 0; j < SIN_MIN_List.length(); j+=2) {
//                        printf("SIN from JSON = %d, SIN from API = %d\n", v.toObject().value("SIN").toInt(), SIN_MIN_List.at(j).toInt());
                        if (v.toObject().value("SIN").toInt() == SIN_MIN_List.at(j).toInt()) {
//                            monita_cfg.sky[indexGateway].mdm[i].val_tu.clear();
                            QJsonArray LoadArray  = v.toObject().value("RawPayload").toArray();
//                            LoadArray.removeAt(0); LoadArray.removeAt(0);
                            if (!LoadArray.isEmpty()) {
//                                if (LoadArray.at(1).toInt() == SIN_MIN_List.at(j+1).toInt()) {
                                    index = 0;
                                    foreach (const QJsonValue & vRaw, LoadArray) {
                                        if (index > 1) {
                                            sprintf(temp, "%c%c%c%c%c%c%c%c", BYTE_TO_BINARY(vRaw.toInt()));
                                            RawBiner.append(temp);
                                        }
                                        index++;
                                    }
                                    PayLoadData = parsingRawPayload(this->format_5cut_32get(RawBiner));
//                                    for (int j = 5; j < PayLoadData.length(); j++) {
//                                        if (PayLoadData.at(j) != "0.000000000") {
//                                            getData = true;
//                                            break;
//                                        }
//                                    }
            //                        listData.append("RawPayload:" + parse.format_5cut_32get(RawBiner));
//                                    if (getData) {
//                                        monita_cfg.sky[indexGateway].mdm[i].last_utc = PayLoadData.at(0);
//                                        if (QDateTime::fromTime_t(monita_cfg.sky[indexGateway].mdm[i].last_utc.toUInt(&ok)).toString("HH").toInt() == 7) {
//                                            qDebug() << "Debug";
//                                        }
                                        PayLoadData.removeAt(0);
                                        QJsonArray raw;
                                        for (int j = 0; j < PayLoadData.length(); j++) {raw.append(PayLoadData.at(j));}
//                                        monita_cfg.sky[indexGateway].mdm[i].val_tu = PayLoadData;
                                        valObject["MobileID"] = v.toObject().value("MobileID").toString();
                                        valObject["ReceiveUTC"] = v.toObject().value("ReceiveUTC").toString();
                                        valObject["Payload"] = raw;
//                                        valObject["SIN"] = v.toObject().value("SIN").toInt();
                                        valObject["SIN"] = LoadArray.at(0).toInt();
                                        valObject["MIN"] = LoadArray.at(1).toInt();

//                                        if (monita_cfg.sky[indexGateway].mdm[i].id_tu.length() > monita_cfg.sky[indexGateway].mdm[i].val_tu.length()) {
//                                            int iVal = monita_cfg.sky[indexGateway].mdm[i].id_tu.length() - monita_cfg.sky[indexGateway].mdm[i].val_tu.length();
//                                            for(int k = 0; k < iVal; k++) {
//                                                monita_cfg.sky[indexGateway].mdm[i].val_tu.insert(monita_cfg.sky[indexGateway].mdm[i].val_tu.length(), "N/A");
//                                            }
//                                        }
//                                        getQuery(MessageUTC, indexGateway, i);
//                                    }
    //                                qDebug() << monita_cfg.sky[indexGateway].mdm[i].query;
                                    RawBiner.clear();
//                                }
                            }
                            if ((v.toObject().value("Payload").toObject().value("SIN").toInt() == SIN_MIN_List.at(j).toInt()) &&
                                    (v.toObject().value("Payload").toObject().value("MIN").toInt() == SIN_MIN_List.at(j+1).toInt())) {
//                                if (v.toObject().value("Payload").toObject().value("Name").toString() == DATA_TRACKING) {
                                if (v.toObject().value("Payload").toObject().value("Name").toString() == "noEIO") {
//                                    monita_cfg.sky[indexGateway].mdm[i].val_tu.clear();
                                    QJsonArray Field_Payload = v.toObject().value("Payload").toObject().value("Fields").toArray();
                                    QJsonArray load;
//                                    for (int j = 0; j < 4; j++) {monita_cfg.sky[indexGateway].mdm[i].val_tu.append("N/A");}
                                    for (int j = 0; j < 4; j++) {load.append("N/A");}
                                    foreach (const QJsonValue vField, Field_Payload) {
//                                        if (vField.toObject().value("Name").toString() == "fixTime") {
//                                            monita_cfg.sky[indexGateway].mdm[i].last_utc = vField.toObject().value("Value").toString();
//                                        } else
                                        if (vField.toObject().value("Name").toString() == "latitude") {
//                                            monita_cfg.sky[indexGateway].mdm[i].val_tu[0] = vField.toObject().value("Value").toString();
//                                            monita_cfg.sky[indexGateway].mdm[i].val_tu[0] = QString::number(monita_cfg.sky[indexGateway].mdm[i].val_tu.at(0).toFloat()/60000.000000000, 'f', 9);
                                            load[0] = QString::number(vField.toObject().value("Value").toString().toFloat()/60000.000000000, 'f', 9);
                                        } else if (vField.toObject().value("Name").toString() == "longitude") {
//                                            monita_cfg.sky[indexGateway].mdm[i].val_tu[1] = vField.toObject().value("Value").toString();
//                                            monita_cfg.sky[indexGateway].mdm[i].val_tu[1] = QString::number(monita_cfg.sky[indexGateway].mdm[i].val_tu.at(1).toFloat()/60000.000000000, 'f', 9);
                                            load[1] = QString::number(vField.toObject().value("Value").toString().toFloat()/60000.000000000, 'f', 9);
                                        }  else if (vField.toObject().value("Name").toString() == "speed") {
//                                            monita_cfg.sky[indexGateway].mdm[i].val_tu[3] = vField.toObject().value("Value").toString();
                                            load[2] = QString::number(vField.toObject().value("Value").toString().toFloat()/10, 'f', 9);
                                        } else if (vField.toObject().value("Name").toString() == "heading") {
//                                            monita_cfg.sky[indexGateway].mdm[i].val_tu[2] = vField.toObject().value("Value").toString();
                                            load[3] = QString::number(vField.toObject().value("Value").toString().toFloat()/10, 'f', 9);
                                        }
                                    }
//                                    if (QDateTime::fromTime_t(monita_cfg.sky[indexGateway].mdm[i].last_utc.toUInt(&ok)).toString("HH").toInt() == 7) {
//                                        qDebug() << "Debug";
//                                    }
//                                    getQuery(MessageUTC, indexGateway, i);
                                    valObject["MobileID"] = v.toObject().value("MobileID").toString();
                                    valObject["ReceiveUTC"] = v.toObject().value("ReceiveUTC").toString();
                                    valObject["Payload"] = load;
                                    valObject["SIN"] = v.toObject().value("Payload").toObject().value("SIN").toInt();
                                    valObject["MIN"] = v.toObject().value("Payload").toObject().value("MIN").toInt();
                                }
                            }
//                            if (!monita_cfg.sky[indexGateway].mdm[i].query.isEmpty()) {
//                                if (monita_cfg.sky[indexGateway].mdm[i].id_tu.length() > monita_cfg.sky[indexGateway].mdm[i].val_tu.length()) {
//                                    for (int j = 0; j < monita_cfg.sky[indexGateway].mdm[i].val_tu.length(); j++) {
////                                        log.write("SkyWave",
////                                                  QString::number(monita_cfg.sky[indexGateway].mdm[i].id_ship) + " - " +
////                                                  monita_cfg.sky[indexGateway].mdm[i].modem_id + " - " +
////                                                  monita_cfg.sky[indexGateway].mdm[i].id_tu.at(j) + " - " +
////                                                  monita_cfg.sky[indexGateway].mdm[i].val_tu.at(j),
////                                                  monita_cfg.config.at(9).toInt());
//                                    }
//                                } else {
//                                    for (int j = 0; j < monita_cfg.sky[indexGateway].mdm[i].id_tu.length(); j++) {
////                                        log.write("SkyWave",
////                                                  QString::number(monita_cfg.sky[indexGateway].mdm[i].id_ship) + " - " +
////                                                  monita_cfg.sky[indexGateway].mdm[i].modem_id + " - " +
////                                                  monita_cfg.sky[indexGateway].mdm[i].id_tu.at(j) + " - " +
////                                                  monita_cfg.sky[indexGateway].mdm[i].val_tu.at(j),
////                                                  monita_cfg.config.at(9).toInt());
//                                    }
//                                }
//                            }
                        }
                    }
//                }
//            }
            if (!valObject["Payload"].isUndefined()) valArray.append(valObject);
        }
//        log.write("SkyWave","Mulai Proses ke Database " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz"), monita_cfg.config.at(10).toInt());
//        QString dataToTable, vTarget, vIDTarget;
//        for (int i = 0; i < monita_cfg.sky[indexGateway].jml_modem; i++) {
//            if (!monita_cfg.sky[indexGateway].mdm[i].query.isEmpty()) {
//                if (monita_cfg.sky[indexGateway].mdm[i].query.indexOf("07:00:00") > 0) {
//                    qDebug() << "Debug";
//                }
//                dataToTable = dataToTable + monita_cfg.sky[indexGateway].mdm[i].query;
//                if (!monita_cfg.sky[indexGateway].mdm[i].last_utc.isEmpty()) {
//                    QString unixTimeStr = monita_cfg.sky[indexGateway].mdm[i].last_utc;
//                    const uint s = unixTimeStr.toUInt( &ok );
//                    const QDateTime dt = QDateTime::fromTime_t( s );
//                    const QString result = dt.toUTC().toString("yyyy-MM-dd HH:mm:ss");
//                    vTarget = vTarget +
//                            "when id_ship = " + QString::number(monita_cfg.sky[indexGateway].mdm[i].id_ship) +
//                            " then '" + result + "' ";
//                    if (vIDTarget.isEmpty()) {
//                        vIDTarget = QString::number(monita_cfg.sky[indexGateway].mdm[i].id_ship);
//                    } else {
//                        vIDTarget = vIDTarget + "," + QString::number(monita_cfg.sky[indexGateway].mdm[i].id_ship);
//                    }
//                }
//                if (i < monita_cfg.sky[indexGateway].jml_modem-1) {
//                    dataToTable = dataToTable + ",";
//                }
//            }
//        }
    //    qDebug() << dataToTable;
//        while (!db_skywave.isOpen()) {
////            db_skywave.close();
//            db_skywave.open();
//            if (!db_skywave.isOpen()) {
//                mysql.close(db_skywave);
////                log.write("Database","Error : Connecting Fail ..!!",
////                          monita_cfg.config.at(10).toInt());
//                QThread::msleep(DELAY_DB_CONNECT);
//                db_skywave = mysql.connect_db("SkyWave");
//            }
//        }
//        if (!dataToTable.isEmpty()) {
//            if (dataToTable.at(dataToTable.length()-1) == ',') {dataToTable.remove(dataToTable.length()-1, 1);}
//            if (!get.check_table_is_available(db_skywave, "data", "SkyWave", monita_cfg.config.at(10).toInt())) {
//                set.create_table_data_punya_skywave(db_skywave, "data", "SkyWave", monita_cfg.config.at(10).toInt());
//            }
//            if (!get.check_table_is_available(db_skywave, "data_" + monita_cfg.sky[indexGateway].next_utc.toString("yyyyMMdd"),
//                                              "SkyWave", monita_cfg.config.at(10).toInt())) {
//                set.create_table_data_punya_skywave(db_skywave, "data_" + monita_cfg.sky[indexGateway].next_utc.toString("yyyyMMdd"),
//                                                    "SkyWave", monita_cfg.config.at(10).toInt());
//            }
//            set.delete_last_utc_punya_skywave(db_skywave, "data", QString::number(QDateTime::currentDateTime().toTime_t() - ((86400 * 2) + (3600 * 7))),
//                                              "SkyWave", monita_cfg.config.at(10).toInt());
//            set.data_punya_skywave(db_skywave, "data", dataToTable, "SkyWave", monita_cfg.config.at(10).toInt());
//            set.data_punya_skywave(db_skywave, "data_" + monita_cfg.sky[indexGateway].next_utc.toString("yyyyMMdd"), dataToTable,
//                                   "SkyWave", monita_cfg.config.at(10).toInt());
//            if (!vTarget.isEmpty()) {
//                set.update_multiple_punya_skywave(db_skywave, "ship", "nextutc", "id_ship", vIDTarget, vTarget,
//                                                  "SkyWave", monita_cfg.config.at(10).toInt());
//            }
//        }
//        if (monita_cfg.sky[indexGateway].next_utc.toString("yyyyMMdd") == QDateTime::fromString(MessageUTC, "yyyy-MM-dd HH:mm:ss").toString("yyyyMMdd")) {
//            monita_cfg.sky[indexGateway].next_utc = QDateTime::fromString(MessageUTC, "yyyy-MM-dd HH:mm:ss");
//        } else {
//            QDateTime dtTemp = monita_cfg.sky[indexGateway].next_utc.addDays(1);
//            monita_cfg.sky[indexGateway].next_utc = QDateTime::fromString(dtTemp.toString("yyyy-MM-dd") + " 00:00:00", "yyyy-MM-dd HH:mm:ss");
//        }
//        set.update_multiple_punya_skywave(db_skywave, "gateway", "next_utc", "id", QString::number(monita_cfg.sky[indexGateway].id_gateWay),
//                                          "when id = " + QString::number(monita_cfg.sky[indexGateway].id_gateWay) + " then '" +
//                                          monita_cfg.sky[indexGateway].next_utc.toString("yyyy-MM-dd HH:mm:ss") + "' ",
//                                          "SkyWave", monita_cfg.config.at(10).toInt());
//        mysql.close(db_skywave);
//        if (db_skywave.isOpen()) db_skywave.close();
//        db_skywave.close();
//        log.write("SkyWave","Selesai .. " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz"), monita_cfg.config.at(9).toInt());
        QJsonObject monita;
        if (!MessageUTC.isEmpty() && !array.isEmpty()) {
            monita["skywave"] = valArray;
            monita["NextStartUTC"] = MessageUTC;
            QJsonDocument doc(monita);
        //    QString formattedJsonString = doc.toJson(QJsonDocument::Indented);
            QString formattedJsonString = doc.toJson(QJsonDocument::Compact);
//            qDebug() << formattedJsonString;
            printf("%s\n\n", formattedJsonString.toLatin1().data());
        } else {
//            qDebug() << "{\"ERR\":\"SkyWave not responding ..\"}";
            printf("{\"ERR\":\"SkyWave not responding ..\"}\n\n");
        }

        emit close();
    } else {
//        log.write("SkyWave","Tidak Dapat Data " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz"), monita_cfg.config.at(9).toInt());
    }
}

void worker::replyFinished(QNetworkReply *reply)
{
    QByteArray data;
    data = reply->readAll();

    this->parsing(data);
    process = 0;
}

void worker::timeOut()
{
    emit close();
}

QString worker::format_5cut_32get(QString biner)
{

    char dat[(biner.size()+1)];

    int c_5bit  = 0;
    int c_32bit = 0;

    bool buang_5bit  = false;
//    bool ambil_32bit = false;

    QString dat_5bit  = ""; /* 5 bit yang di buang */
    QString dat_32bit = ""; /* 32 bit yang di ambil */
    QString all_32bit = ""; /* gabung semua 32 bit */

    strcpy(dat, biner.toLatin1());
    for (int i = 0; i < biner.size(); i++){
        if (buang_5bit == false){
            dat_5bit.sprintf("%s%c", dat_5bit.toLocal8Bit().data(), dat[i]);

            if (c_5bit == 4){
                buang_5bit = true;
                c_32bit = 0;
                dat_5bit = "";
            }
            c_5bit++;
        }
        else if(buang_5bit == true){
            dat_32bit.sprintf("%s%c", dat_32bit.toLocal8Bit().data(), dat[i]);

            if (c_32bit == 31){
                all_32bit.sprintf("%s%s", all_32bit.toLocal8Bit().data(), dat_32bit.toLocal8Bit().data());
                buang_5bit = false;
                c_5bit = 0;
                dat_32bit = "";
            }
            c_32bit++;
        }
    }
    return (QString) all_32bit;
}

QStringList worker::parsingRawPayload(QString RawData)
{
    bool ok;
    int cnt_p = 0; int cnt_d = 0;
    QString data;
    QString data_decimal; QString data_real;
    QStringList DataSkyWave;
    data.clear();

    char dats[RawData.size()+1];
    strcpy(dats, RawData.toLatin1());
    for (int j = 0; j < RawData.size(); j++) {
        cnt_p++;
        data.sprintf("%s%c", data.toLocal8Bit().data(), dats[j]);
        if (cnt_p == 32) {
            cnt_d++;
            unsigned int data_bin = data.toUInt(&ok, 2);
            float data_float = (*(float *) &data_bin);
            data_real = QString::number(data_float, 'f', 9);
            data_decimal = QString::number(data_float, 'f', 0);
            if (cnt_d == 1) {
                DataSkyWave.append(data_decimal);
            } else {
                DataSkyWave.append(data_real);
            }
            data.clear();
            cnt_p = 0;
        }
    }
    return DataSkyWave;
}
