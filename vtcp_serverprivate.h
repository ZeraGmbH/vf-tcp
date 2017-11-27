#ifndef VEIN_TCP_SERVERPRIVATE_H
#define VEIN_TCP_SERVERPRIVATE_H

#include <QList>

namespace VeinTcp
{
  class TcpPeer;
  class TcpServer;

  /**
 * @brief XiQNetServer PIMPL
 */
  class TcpServerPrivate
  {
    TcpServerPrivate(TcpServer *t_public);
    ~TcpServerPrivate();

    QList<TcpPeer*> m_clients;
    TcpServer *q_ptr = 0;

    friend class TcpServer;
  };
}
#endif // VEIN_TCP_SERVERPRIVATE_H
