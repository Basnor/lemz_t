#include <QCoreApplication>

#include <src/DspClient.h>
#include <src/Recorder.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString ip = "127.0.0.1";
    uint16_t port = 10600;
    QString path = "dspRec";

    if (argc > 1)
    {
        ip = QString(argv[1]);
        port = QString(argv[2]).toInt();
        path = QString(argv[3]);
    }

    DspClient tcp;
    Recorder* rec = new Recorder();
    tcp.SetConnectionCallback(rec);
    tcp.connect(ip, port);
    rec->setRecordsBankPath(path);
    return a.exec();
}

