#include <QCoreApplication>
#include <QTranslator>
#include <QDebug>

#include <src/DspClient.h>
#include <src/Recorder.h>
#include <src/ParamParser.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ParamParser paramParser(argc, argv);

    QMap<QString, QString> params;
    params.insert("ip", "127.0.0.1");
    params.insert("port", "10600");
    params.insert("path", "dspRec");
    params.insert("lang", "eng");

    //поиск языкового ключа
    QTranslator t;
    if (paramParser.findLang(params))
    {
        if (params["lang"] == "rus")
        {
            t.load(":/tr/recorder_rus.qm");
            a.installTranslator(&t);
        }
    }

    //поиск ключа справки
    if (!paramParser.findHelp()) return 0;

    //проверка ключей
    if (!paramParser.parseParams(params)) return 0;

    DspClient tcp;
    Recorder* rec = new Recorder(&tcp);
    rec->setRecordsBankPath(params["path"]);

    tcp.SetConnectionCallback(rec);
    tcp.connect(params["ip"], params["port"].toInt());

    return a.exec();
}

