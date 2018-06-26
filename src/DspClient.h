#pragma once


#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class QTcpSocket;
class QTimer;


struct ITCPConnectionCallback
{
    virtual void OnConnected() = 0;
    virtual void OnConnectionFailed() = 0;
    virtual void OnDisconnected() = 0;
    virtual void OnData(const uint8_t*, size_t) = 0;
};


class DspClient : public QObject
{
    Q_OBJECT


public:
    DspClient(QObject * parent = nullptr);

//    void send(const uint8_t* data, size_t size) override;

    void connect(const QString& ip, uint16_t port);
    void disconnect();

    void SetConnectionCallback(ITCPConnectionCallback* callback)
    {
        m_connectionCallback = callback;
    }


private:
    void reconnect();


private:
    QTcpSocket* socket;
    QString ip;
    uint16_t port;

    QTimer* reconnectTimer = nullptr;

    ITCPConnectionCallback* m_connectionCallback = nullptr;
};
