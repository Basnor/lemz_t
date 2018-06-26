#pragma once


#include <QDir>

#include "DspClient.h"


class QTimer;


class Recorder : public QObject
               , public ITCPConnectionCallback
{
    Q_OBJECT

public:
    Recorder(QObject* parent = nullptr);
    ~Recorder();

    void OnConnected() override;
    void OnConnectionFailed() override;
    void OnDisconnected() override;
    void OnData(const uint8_t* data, size_t size) override;

    void record(const char* data, size_t);

    void setRecordsBankPath(const QString& path);
    QString recordsBankPath() const;

    void setFileExtension(const QString& extension);
    QString fileExtension() const;


private:
    void deleteOldData();

private slots:
    void alarm();
    bool newFileRecord();

 private:
    QDir m_recordsBankPath;
    QFile* m_file;
    const QString format = "yyyy.MM.dd";
    QString extention = "dat";

    QTimer* timer;
};
