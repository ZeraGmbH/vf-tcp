#ifndef PROTOPEERPRIVATE_H
#define PROTOPEERPRIVATE_H

#include "xiqnetwrapper.h"

#include <QUuid>

class XiQNetPeer;

class QTcpSocket;

/**
 * @brief XiQNetPeer PIMPL
 */
class XiQNetPeerPrivate
{
private:
  explicit XiQNetPeerPrivate(XiQNetPeer *t_publicPeer);

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

  int m_peerId = -1;

  QTcpSocket *m_tcpSock = 0;
  XiQNetPeer *q_ptr = 0;

  friend class XiQNetPeer;
};

#endif // PROTOPEERPRIVATE_H
