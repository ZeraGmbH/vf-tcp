#include "vtcp_server.h"
#include "vtcp_serverprivate.h"

#include "vtcp_peer.h"


namespace VeinTcp
{
  TcpServer::TcpServer(QObject *t_parent) :
    QTcpServer(t_parent),
    d_ptr(new TcpServerPrivate(this))
  {
  }

  TcpServer::~TcpServer()
  {
    delete d_ptr;
  }

  QList<TcpPeer *> TcpServer::getClientList() const
  {
    return d_ptr->m_clients;
  }

  void TcpServer::broadcastMessage(QByteArray t_message) const
  {
    for(TcpPeer *tmpClient : d_ptr->m_clients)
    {
      tmpClient->sendMessage(t_message);
    }
  }

  void TcpServer::startServer(quint16 t_port)
  {
    if(this->listen(QHostAddress::Any, t_port))
    {
      qDebug()<<"[xiqnet-qt]Server Started on port:" << t_port;
    }
    else
    {
      qCritical() << "[xiqnet-qt]Server could not listen on port:" << t_port << "error:" << errorString();
    }
  }

  void TcpServer::clientDisconnectedSRV()
  {
    Q_ASSERT(QObject::sender()!=0); /// @todo redesign to not rely on QObject::sender
    TcpPeer *client = qobject_cast<TcpPeer*>(QObject::sender());
    Q_ASSERT(client != 0);

    d_ptr->m_clients.removeAll(client);
    ///@note use deletelater to execute other signal slot connections connected to the XiQNetPeer::sigConnectionClosed signal
    client->deleteLater();
  }

  void TcpServer::incomingConnection(qintptr t_socketDescriptor)
  {
    qDebug()<<"[xiqnet-qt]Client connected";

    TcpPeer *client = new TcpPeer(t_socketDescriptor, this);
    d_ptr->m_clients.append(client);
    connect(client, &TcpPeer::sigConnectionClosed, this, &TcpServer::clientDisconnectedSRV);
    emit sigClientConnected(client);
  }
}
