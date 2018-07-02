#include "src/Recorder.h"

#include <chrono>
#include <iostream>

#include <QDateTime>
#include <QTimer>
#include <QDataStream>
#include <QDebug>


Recorder::Recorder(QObject *parent)
    : QObject(parent)
    , m_file(new QFile(this))
{
//    setRecordsBankPath("records");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(alarm()));
    connect(timer, SIGNAL(timeout()), this, SLOT(newFileRecord()));

    alarm();

    timer->start();
}

Recorder::~Recorder()
{
    m_file->close();
}

void Recorder::OnConnected()
{
    std::cout << QTime::currentTime().toString("hh.mm.ss").toUtf8().constData() << " Connected" << std::endl;
}

void Recorder::OnConnectionFailed()
{
    std::cout << QTime::currentTime().toString("hh.mm.ss").toUtf8().constData() << " ConnectionFailed" << std::endl;
}

void Recorder::OnDisconnected()
{
    std::cout << QTime::currentTime().toString("hh.mm.ss").toUtf8().constData() << " Disconnected" << std::endl;
}

void Recorder::OnData(const uint8_t* data, size_t size)
{
    record(reinterpret_cast<const char*>(data), size);
}

void Recorder::record(const char* data, size_t size)
{
    if (!m_file->isOpen())
        if (newFileRecord() == false)
            return;

    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    // Пишем метку времени
    qint64 now = std::chrono::system_clock::now().time_since_epoch().count();
    stream << now;
    stream.writeBytes(data, size);

    m_file->write(buffer);
}

void Recorder::alarm()
{
    QTime now = QTime::currentTime();
    now.setHMS(0, now.minute(), now.second(), now.msec());
    QTime nextHour(now.hour() + 1, 0, 0);
    timer->setInterval( now.msecsTo(nextHour) );
}

void Recorder::deleteOldData()
{
    QStringList dirList = m_recordsBankPath.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    QDate oneMonthAgo = QDate::currentDate().addMonths(-1);

    for (QString & name : dirList)
    {
        QDate dateFromName = QDate::fromString(name, format);
        if (dateFromName <= oneMonthAgo)
        {
            m_recordsBankPath.cd(name);
            QStringList fileList = m_recordsBankPath.entryList(QDir::NoDotAndDotDot | QDir::Files);
            for (auto & file : fileList)
                m_recordsBankPath.remove(file);
            m_recordsBankPath.cdUp();
            m_recordsBankPath.rmpath(name);
        }
    }
}

bool Recorder::newFileRecord()
{
    QString dir = QDate::currentDate().toString(format);

    if (false == m_recordsBankPath.exists( dir ))
    {
        m_recordsBankPath.mkdir( dir );
    }
    deleteOldData();

    m_file->close();

    QString filename = QTime::currentTime().toString("hh.mm.ss") + "." + extention;

    m_file->setFileName(m_recordsBankPath.path() + "/" + dir + "/" + filename);
    if(m_file->open(QIODevice::WriteOnly))
    {
        std::cout << QTime::currentTime().toString("hh.mm.ss").toUtf8().constData() << " Start record in " << m_file->fileName().toStdString() << std::endl;
        return true;
    }
    else
    {
        std::cout << QTime::currentTime().toString("hh.mm.ss").toUtf8().constData() << " File not open" << m_file->fileName().toStdString() << std::endl;
        return false;
    }
}

QString Recorder::recordsBankPath() const
{
    return m_recordsBankPath.absolutePath();
}

void Recorder::setRecordsBankPath(const QString &path)
{
    if (!m_recordsBankPath.exists(path))
    {
        m_recordsBankPath.mkpath(path);
    }

    m_recordsBankPath.setPath(path);
}

void Recorder::setFileExtension(const QString &extension)
{
    this->extention = extension;
}

QString Recorder::fileExtension() const
{
    return extention;
}
