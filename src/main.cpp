#include <QCoreApplication>
#include <QTranslator>
#include <QDebug>

#include <src/DspClient.h>
#include <src/Recorder.h>
#include <src/ParamParser.h>


#include <iostream>
#include <iomanip>
#include <ios>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ParamParser paramParser(argc, argv);

    QMap<QString, QString> params;
    params.insert("ip", "127.0.0.1");
    params.insert("port", "10600");
    params.insert("path", "dspRec");

    QTranslator t;
    int lang = 1;

    if (lang == 1)
    {

        t.load(":/tr/recorder_rus.qm");
        //t.load("recorder_rus", "tr");
        a.installTranslator(&t);
    }

    /*std::cout << "НАЧАЛЬНЫЕ ЗНАЧЕНИЯ"  << std::endl;
    std::cout << params["ip"].toUtf8().constData()  << std::endl;
    std::cout << params["port"].toUtf8().constData()  << std::endl;
    std::cout << params["path"].toUtf8().constData()  << std::endl;*/

    if (!paramParser.findHelp()) return 0;

    if (!paramParser.parseParams(params)) return 0;

    /*// отслеживание введённых ключей
    std::cout << argc << std::endl;
    for (int i = 1; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }

    std::cout << "КОНЕЧНЫЕ ЗНАЧЕНИЯ"  << std::endl;
    std::cout << params["ip"].toUtf8().constData()  << std::endl;
    std::cout << params["port"].toUtf8().constData()  << std::endl;
    std::cout << params["path"].toUtf8().constData()  << std::endl;*/

    DspClient tcp;
    Recorder* rec = new Recorder(&tcp);
    rec->setRecordsBankPath(params["path"]);

    tcp.SetConnectionCallback(rec);
    tcp.connect(params["ip"], params["port"].toInt());

    return a.exec();
}

