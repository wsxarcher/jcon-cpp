#include "json_rpc_websocket.h"
#include "jcon_assert.h"

#include <QSignalSpy>
#include <QTime>
#include <QWebSocket>

namespace jcon {

JsonRpcWebSocket::JsonRpcWebSocket()
    : m_socket(new QWebSocket)
{
    setupSocket();
}

JsonRpcWebSocket::JsonRpcWebSocket(QWebSocket* socket)
    : m_socket(socket)
{
    setupSocket();
}

JsonRpcWebSocket::~JsonRpcWebSocket()
{
    m_socket->disconnect(this);
    m_socket->deleteLater();
}

void JsonRpcWebSocket::setupSocket()
{
    connect(m_socket, &QWebSocket::textMessageReceived,
            this, &JsonRpcWebSocket::dataReady);

    connect(m_socket, &QWebSocket::disconnected, [this]() {
        emit socketDisconnected(m_socket);
    });
}

void JsonRpcWebSocket::connectToHost(QString host, int port)
{
    QUrl url;
    url.setScheme("ws");
    url.setHost(host);
    url.setPort(port);
    m_socket->open(url);
}

bool JsonRpcWebSocket::waitForConnected(int msecs)
{
    QTime timer(0, 0, 0, msecs);
    QSignalSpy spy(m_socket, &QWebSocket::connected);
    timer.start();
    while (spy.isEmpty() && timer.elapsed() < msecs) {
        QCoreApplication::processEvents();
    }
    return !spy.isEmpty();
}

void JsonRpcWebSocket::disconnectFromHost()
{
    m_socket->close();
}

bool JsonRpcWebSocket::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void JsonRpcWebSocket::send(const QByteArray& data)
{
    m_socket->sendTextMessage(data);
}

QString JsonRpcWebSocket::errorString() const
{
    return m_socket->errorString();
}

QHostAddress JsonRpcWebSocket::localAddress() const
{
    return m_socket->localAddress();
}

int JsonRpcWebSocket::localPort() const
{
    return m_socket->localPort();
}

QHostAddress JsonRpcWebSocket::peerAddress() const
{
    return m_socket->peerAddress();
}

int JsonRpcWebSocket::peerPort() const
{
    return m_socket->peerPort();
}

void JsonRpcWebSocket::dataReady(const QString& data)
{
    emit dataReceived(data.toUtf8(), m_socket);
}

}