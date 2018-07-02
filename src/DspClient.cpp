#include "src/DspClient.h"

DspClient::DspClient(QObject *parent)
    : QObject(parent)
    , socket(new QTcpSocket(this))
    , reconnectTimer(new QTimer(this))
{
    QObject::connect(socket, &QIODevice::readyRead, [this]()
    {
        if (m_connectionCallback)
        {
            auto data = socket->readAll();
            m_connectionCallback->OnData(reinterpret_cast<const uint8_t*>(data.data()), data.size());
        }
    });
    QObject::connect(socket, &QTcpSocket::connected, [this]
    {
        if (m_connectionCallback)
            m_connectionCallback->OnConnected();
    });
    QObject::connect(socket, &QTcpSocket::disconnected, [this]
    {
        if (m_connectionCallback)
            m_connectionCallback->OnDisconnected();
    });
    QObject::connect(socket,
            static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            [this]
    {
        if (m_connectionCallback)
            m_connectionCallback->OnConnectionFailed();
        reconnect();
    });

    reconnectTimer->setSingleShot(true);
    reconnectTimer->setInterval(1000);
    QObject::connect(reconnectTimer, &QTimer::timeout, [this]
    {
        socket->connectToHost(ip, port);
    });
}


void DspClient::connect(const QString &ip, uint16_t port)
{
    this->ip = ip;
    this->port = port;
    socket->connectToHost(ip, port);
}


void DspClient::disconnect()
{
    socket->disconnectFromHost();
}


void DspClient::reconnect()
{
    if (socket->isValid())
        socket->disconnectFromHost();

    reconnectTimer->start();
}


/*void DspClient::send(const uint8_t *data, size_t size)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
        return;

    socket->write(reinterpret_cast<const char*>(data), size);
}*/
