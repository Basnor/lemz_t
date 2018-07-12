#include <QCoreApplication>
#include <QDebug>
#include <QTranslator>

#include <src/DspClient.h>
#include <src/Recorder.h>
#include <src/ParamParser.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTranslator t;
    if (std::string(getenv("LANG")) == "en_US.UTF-8")
        ;
    else if (std::string(getenv("LANG")) == "ru_RU.UTF-8")
    {
        t.load(":/tr/recorder_rus.qm");
        a.installTranslator(&t);
    }
    //else std::cout << "Неверное значение LANG" << std::endl;

    ParamParser paramParser(argc, argv);
    DspClient tcp;
    Recorder* rec = new Recorder(&tcp);

    if (!paramParser.findHelp())
    {
        if (paramParser.isParamsCorrect)
        {
            rec->setRecordsBankPath(paramParser.getPath());

            tcp.SetConnectionCallback(rec);
            tcp.connect(paramParser.getIp(), paramParser.getPort());
            return a.exec();
        }
    }
    else
    {
        paramParser.showHelp();
        return 0;
    }
}

