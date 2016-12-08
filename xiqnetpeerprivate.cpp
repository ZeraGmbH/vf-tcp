#include "xiqnetpeerprivate.h"

#include <QDataStream>
#include <QTcpSocket>


XiQNetPeerPrivate::XiQNetPeerPrivate(XiQNetPeer *t_publicPeer) :  q_ptr(t_publicPeer)
{
}

QByteArray XiQNetPeerPrivate::readArray()
{
  // it is at least required to read the expected size
    QDataStream in(m_tcpSock);
    QByteArray retVal;
    in.setVersion(QDataStream::Qt_4_0);

    in.startTransaction();

    in >> retVal;

    if (in.commitTransaction() == true)
    {
      return retVal;
    }
    else //need to wait for more data
    {
      return QByteArray();
    }
}

void XiQNetPeerPrivate::sendArray(const QByteArray &t_byteArray) const
{
  Q_ASSERT(m_tcpSock != 0 && m_tcpSock->isOpen());

  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);

  out << t_byteArray;

  if(m_tcpSock->write(block) < block.size())
  {
    qWarning() << "[xiqnet-qt] could not send all data, the network is congested";
  }
}
