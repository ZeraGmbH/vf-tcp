#ifndef VEIN_TCP_PEERPRIVATE_H
#define VEIN_TCP_PEERPRIVATE_H

#include <QUuid>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace VeinTcp
{
  class TcpPeer;


  /**
 * @brief XiQNetPeer PIMPL
 */
  class TcpPeerPrivate
  {
  private:
    explicit TcpPeerPrivate(TcpPeer *t_publicPeer);

    /**
   * @brief readClient
   * @return
   */
    QByteArray readArray() const;

    /**
   * @brief sendByteArray
   * @param[in] t_byteArray Data that will be sent
   */
    void sendArray(const QByteArray &t_byteArray) const;

    QUuid m_peerId;

    QTcpSocket *m_tcpSock = 0;
    TcpPeer *q_ptr = 0;

    friend class TcpPeer;
  };
}
#endif // VEIN_TCP_PEERPRIVATE_H
