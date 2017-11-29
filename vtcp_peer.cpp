#include "vtcp_peer.h"
#include "vtcp_peerprivate.h"
#include <QTcpSocket>
#include <QHostAddress>

namespace VeinTcp
{
  TcpPeer::TcpPeer(QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new TcpPeerPrivate(this))
  {
  }

  TcpPeer::TcpPeer(qintptr t_socketDescriptor, QObject *t_parent) :
    QObject(t_parent),
    d_ptr(new TcpPeerPrivate(this))
  {
    d_ptr->m_tcpSock = new QTcpSocket();
    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, &TcpPeer::sigConnectionEstablished);
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, [this](){ emit sigConnectionClosed(this); });
    connect(d_ptr->m_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));

    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &TcpPeer::stopConnection);
    if(!d_ptr->m_tcpSock->setSocketDescriptor(t_socketDescriptor))
    {
      emit sigSocketError(d_ptr->m_tcpSock->error());
      qFatal("[xiqnet-qt] Error setting clients socket descriptor");
    }
    d_ptr->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
  }

  TcpPeer::~TcpPeer()
  {
    //From the Qt manual QAbstractSocket::disconnected(): "Warning: If you need to delete the sender() of this signal in a slot connected to it, use the deleteLater() function."
    //The destructor will be called in such a case so delete the QTcpSocket with deleteLater()
    d_ptr->m_tcpSock->deleteLater();
    d_ptr->m_tcpSock=0;
    delete d_ptr;
    d_ptr=0;
  }

  QString TcpPeer::getIpAddress() const
  {
    return d_ptr->m_tcpSock->peerAddress().toString();
  }

  quint16 TcpPeer::getPort() const
  {
    return d_ptr->m_tcpSock->peerPort();
  }

  bool TcpPeer::isConnected() const
  {
    return d_ptr->m_tcpSock && (d_ptr->m_tcpSock->state()==QTcpSocket::ConnectedState || d_ptr->m_tcpSock->state()==QTcpSocket::BoundState);
  }

  QUuid TcpPeer::getPeerId() const
  {
    return d_ptr->m_peerId;
  }

  void TcpPeer::setPeerId(QUuid t_peerId)
  {
    Q_ASSERT(t_peerId.isNull() == false);

    d_ptr->m_peerId = t_peerId;
  }

  QTcpSocket *TcpPeer::getTcpSocket() const
  {
    return d_ptr->m_tcpSock;
  }

  QString TcpPeer::getErrorString() const
  {
    return d_ptr->m_tcpSock->errorString();
  }

  void TcpPeer::sendMessage(QByteArray t_message) const
  {
    Q_ASSERT_X(isConnected(), __PRETTY_FUNCTION__, "[xiqnet-qt] Trying to send data to disconnected host.");

    d_ptr->sendArray(t_message);
  }

  void TcpPeer::startConnection(QString t_ipAddress, quint16 t_port)
  {
    //the tcp socket must not exist at this point
    Q_ASSERT_X(d_ptr->m_tcpSock==0, __PRETTY_FUNCTION__, "[xiqnet-qt] Do not re-use XiqNetPeer instances.");

    d_ptr->m_tcpSock= new QTcpSocket(this);

    connect(d_ptr->m_tcpSock, &QTcpSocket::connected, this, &TcpPeer::sigConnectionEstablished);
    connect(d_ptr->m_tcpSock, &QTcpSocket::readyRead, this, &TcpPeer::onReadyRead);
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, [this](){ emit sigConnectionClosed(this); });
    connect(d_ptr->m_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));
    connect(d_ptr->m_tcpSock, &QTcpSocket::disconnected, this, &TcpPeer::stopConnection);
    d_ptr->m_tcpSock->connectToHost(t_ipAddress, t_port);
    d_ptr->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
  }

  void TcpPeer::stopConnection()
  {
    Q_ASSERT(d_ptr->m_tcpSock);

    d_ptr->m_tcpSock->close();
    //qDebug() << "disconnected";
  }

  void TcpPeer::onReadyRead()
  {
    QByteArray newMessage;
    newMessage = d_ptr->readArray();
    while(!newMessage.isNull())
    {
      //qDebug() << "[xiqnet-qt] Message received: "<<newMessage.toBase64();
      emit sigMessageReceived(newMessage);
      newMessage = d_ptr->readArray();
    }
  }
}
