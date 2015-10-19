#ifndef JSONRPCTCPSOCKET_H
#define JSONRPCTCPSOCKET_H

#include "jcon.h"
#include "json_rpc_socket.h"

#include <QTcpSocket>

namespace jcon {

class JCON_API JsonRpcTcpSocket : public JsonRpcSocket
{
    Q_OBJECT

public:
    /**
     * Default constructor. Create a new QTcpSocket.
     */
    JsonRpcTcpSocket();

    /**
     * Constructor taking a previously created socket. This is used by
     * JsonRpcServer, since QTcpServer::nextPendingConnection() returns an
     * already created socket for the client connection.
     *
     * @param[in] socket The TCP socket to use.
     */
    JsonRpcTcpSocket(QTcpSocket* socket);

    virtual ~JsonRpcTcpSocket();

    void connectToHost(QString host, int port) override;
    bool waitForConnected(int msecs) override;
    void disconnectFromHost() override;
    bool isConnected() const override;
    void send(const QByteArray& data) override;
    QString errorString() const override;
    QHostAddress localAddress() const override;
    int localPort() const override;
    QHostAddress peerAddress() const override;
    int peerPort() const override;

private slots:
    void dataReady();

private:
    void setupSocket();

    QTcpSocket* m_socket;
};

}

#endif
