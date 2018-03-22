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

  bool TcpServer::startServer(quint16 t_port)
  {
    bool retVal = false;
    if(this->listen(QHostAddress::Any, t_port))
    {
      retVal = true;
      qDebug()<<"[vein-tcp]Server Started on port:" << t_port;
    }
    else
    {
      qCritical() << "[vein-tcp]Server could not listen on port:" << t_port << "error:" << errorString();
    }
    return retVal;
  }

  void TcpServer::clientDisconnectedSRV(TcpPeer *t_peer)
  {
    d_ptr->m_clients.removeAll(t_peer);
    ///@note use deletelater to execute other signal slot connections connected to the XiQNetPeer::sigConnectionClosed signal
    t_peer->deleteLater();
  }

  void TcpServer::incomingConnection(qintptr t_socketDescriptor)
  {
    qDebug()<<"[vein-tcp]Client connected";

    TcpPeer *client = new TcpPeer(t_socketDescriptor, this); //deleted in TcpServer::clientDisconnectedSRV
    d_ptr->m_clients.append(client);
    connect(client, &TcpPeer::sigConnectionClosed, this, &TcpServer::clientDisconnectedSRV);
    emit sigClientConnected(client);
  }
}
