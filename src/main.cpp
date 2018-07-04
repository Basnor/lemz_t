#include <QCoreApplication>
#include <QDebug>
#include <QTranslator>

#include <src/DspClient.h>
#include <src/Recorder.h>
#include <src/ParamParser.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ParamParser paramParser(argc, argv);


    QTranslator t;
    if (!QString::localeAwareCompare(paramParser.getLang(), "rus"))
    {
        t.load(":/tr/recorder_rus.qm");
        a.installTranslator(&t);
    }

    DspClient tcp;
    Recorder* rec = new Recorder(&tcp);

    if (!paramParser.getHelp())
    {
        rec->setRecordsBankPath(paramParser.getPath());

        tcp.SetConnectionCallback(rec);
        tcp.connect(paramParser.getIp(), paramParser.getPort());
    }

   return a.exec();
}

